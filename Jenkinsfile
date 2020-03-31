// This file is part of jack_playrec
// Copyright (C) 2019 2020 HörTech gGmbH
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

// On the 2019 windows build server, we cannot use the sh step anymore.
// This workaround invokes the msys2 bash, sets the required environment
// variables, and executes the desired command.
def windows_bash(command) {
  bat ('C:\\msys64\\usr\\bin\\bash -c "source /jenkins.environment && set -ex && ' + command + ' "')
  // This will probably fail if command contains multiple lines, quotes, or
  // similar.  Currently all our shell commands are simple enough for this
  // simple solution to work.  Should this no longer be sufficient, then we
  // could write the shell command to a temporary file and execute this file
  // after sourcing the enviroment.
}

// Encapsulation of the build steps to perform when building jack_playrec
// @param stage_name the stage name is "system && arch && devenv" where "system"
//                   describes the target operating system and can have values
//                   like "bionic", "xenial", "windows", or "mac".   "arch" is
//                   either "x86_64", "i686", or "armv7".  "devenv" specifies
//                   the development environment, we use "mhadev" for
//                   jack_playrec. system, arch and devenv are separated by an
//                   "&&" operator and spaces.  This string is also used as a
//                   valid label expression for jenkins.  The appropriate nodes
//                   have the respective labels.
def jack_playrec_steps(stage_name) {
  // Extract components from stage_name:
  def system, arch, devenv
  (system,arch,devenv) = stage_name.split(/ *&& */) // variable number of spaces

  // platform booleans
  def linux = (system != "windows" && system != "mac")
  def windows = (system == "windows")
  def mac = (system == "mac")

  // workaround to invoke unix shell on all systems
  def bash = { command -> windows ? windows_bash(command) : sh(command) }

  // checkout jack_playrec from version control system, the exact same revision that
  // triggered this job on each build slave
  checkout scm

  // Avoid that artifacts from previous builds influence this build
  bash "git reset --hard && git clean -ffdx"

  // Autodetect libs/compiler
  bash "./configure"

  // platform specific installer creation targets
  def pkgs = mac ? " pkg" : ""
  def debs = linux ? " deb" : ""
  def exes = windows ? " exe" : ""
  bash ("make all" + debs + exes + pkgs)

  if (linux) {
    // Store debian packets for later retrieval by the repository manager
    stash name: (arch+"_"+system), includes: 'packaging/deb/hoertech/'
  }

 if (mac) {
    // Store macOS packets for later retrieval by the uploader
    stash name: (arch+"_"+system), includes: 'packaging/pkg/'
  }

  if (windows) {
    // Store windows installer packets for later retrieval by the repository manager
    stash name: (arch+"_"+system), includes: 'packaging/exe/'
  }
}

pipeline {
    agent {label "jenkinsmaster"}
    stages {
        stage("build") {
            parallel {
                stage(                        "bionic && x86_64 && mhadev") {
                    agent {label              "bionic && x86_64 && mhadev"}
                    steps {jack_playrec_steps("bionic && x86_64 && mhadev")}
                }
                stage(                        "xenial && x86_64 && mhadev") {
                    agent {label              "xenial && x86_64 && mhadev"}
                    steps {jack_playrec_steps("xenial && x86_64" && mhadev)}
                }
                stage(                        "focal && x86_64 && mhadev") {
                    agent {label              "focal && x86_64 && mhadev"}
                    steps {jack_playrec_steps("focal && x86_64 && mhadev")}
                }
                stage(                        "bionic && armv7 && mhadev") {
                    agent {label              "bionic && armv7 && mhadev"}
                    steps {jack_playrec_steps("bionic && armv7 && mhadev")}
                }
                stage(                        "windows && x86_64 && mhadev") {
                    agent {label              "windows && x86_64 && mhadev"}
                    steps {jack_playrec_steps("windows && x86_64 && mhadev")}
                }
                stage(                        "mac && x86_64 && mhadev") {
                    agent {label              "mac && x86_64 && mhadev"}
                    steps {jack_playrec_steps("mac && x86_64 && mhadev")}
                }
            }
        }
        stage("publish") {
            agent {label "aptly"}
            steps {
                // Receive all deb packages from jack_playrec build
                unstash "x86_64_focal"
                unstash "x86_64_bionic"
                unstash "x86_64_xenial"
                unstash "armv7_bionic"

                archiveArtifacts 'packaging/deb/hoertech/*/*deb'

                // Make windows installer available as a Jenkins artifact
                unstash "x86_64_windows"
                archiveArtifacts 'packaging/exe/*exe'

                // Make the macOS installer available as a Jenkins artifact
                unstash "x86_64_mac"
                archiveArtifacts 'packaging/pkg/*pkg'
            }
        }
    }

    // Email notification on failed build taken from
    // https://jenkins.io/doc/pipeline/tour/post/
    // multiple recipients are comma-separated:
    // https://jenkins.io/doc/pipeline/steps/workflow-basic-steps/#-mail-%20mail
    post {
        failure {
            mail to: 'p.maanen@hoertech.de,t.herzke@hoertech.de,g.grimm@hoertech.de',
                 subject: "Failed Pipeline: ${currentBuild.fullDisplayName}",
                 body: "Something is wrong with ${env.BUILD_URL}"
        }
    }
}
