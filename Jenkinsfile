// This file is part of jack_playrec
// Copyright (C) 2019 HörTech gGmbH
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

def jack_playrec_build_steps(stage_name) {
  // Extract components from stage_name:
  def system, arch
  (system,arch) = stage_name.split(/ *&& */) // regexp for missing/extra spaces

  // checkout jack_playrec from version control system, the exact same revision that
  // triggered this job on each build slave
  checkout scm

  // Avoid that artifacts from previous builds influence this build
  sh "git reset --hard && git clean -ffdx"

  // Autodetect libs/compiler
  sh "./configure"

  // On linux, we also create debian packages
  def linux = (system != "windows" && system != "mac")
  def windows = (system == "windows")
  def mac = (system == "mac")
  def pkgs = mac ? " pkg" : ""
  def debs = linux ? " deb" : ""
  def exes = windows ? " exe" : ""
  sh ("make all" + debs + exes + pkgs)

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
                stage(                         "bionic && x86_64") {
                    agent {label               "bionic && x86_64"}
                    steps {jack_playrec_build_steps("bionic && x86_64")}
                }
                stage(                         "xenial && x86_64") {
                    agent {label               "xenial && x86_64"}
                    steps {jack_playrec_build_steps("xenial && x86_64")}
                }
                stage(                         "trusty && x86_64") {
                    agent {label               "trusty && x86_64"}
                    steps {jack_playrec_build_steps("trusty && x86_64")}
                }
                stage(                         "bionic && armv7") {
                    agent {label               "bionic && armv7"}
                    steps {jack_playrec_build_steps("bionic && armv7")}
                }
                stage(                         "xenial && armv7") {
                    agent {label               "xenial && armv7"}
                    steps {jack_playrec_build_steps("xenial && armv7")}
                }
                stage(                         "windows && x86_64") {
                    agent {label               "windows && x86_64"}
                    steps {jack_playrec_build_steps("windows && x86_64")}
                }
                stage(                         "mac && x86_64") {
                    agent {label               "mac && x86_64"}
                    steps {jack_playrec_build_steps("mac && x86_64")}
                }
            }
        }
        stage("publish") {
            agent {label "aptly"}
            // // do not publish packages for any branches except these
            // when { anyOf { branch 'master'; branch 'development'; branch 'feature/automatic-build-jobs'} }
            steps {
            //     checkout([$class: 'GitSCM', branches: [[name: "master"]], doGenerateSubmoduleConfigurations: false, extensions: [[$class: 'CleanCheckout']], submoduleCfg: [], userRemoteConfigs: [[url: "ssh://mha.physik.uni-oldenburg.de/openMHA-aptly"]]])

                // Receive all deb packages from jack_playrec build
                unstash "x86_64_bionic"
                unstash "x86_64_xenial"
                unstash "x86_64_trusty"
                unstash "armv7_bionic"
                unstash "armv7_xenial"

                // Copies the new debs to the stash of existing debs,
                // creates an apt repository, uploads.
                //sh "make"

                // For now, make the windows installer available in a tar file that we publish
                // as a Jenkins artifact
                unstash "x86_64_windows"
                //sh "tar cvzf windows-installer.tar.gz mha/tools/packaging/exe/*exe"
                archiveArtifacts 'packaging/exe/*exe'

                // For now, make the macOS installer available in a tar file that we publish
                // as a Jenkins artifact
                unstash "x86_64-mac"
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
            mail to: 'p.maanen@hoertech.de',
                 subject: "Failed Pipeline: ${currentBuild.fullDisplayName}",
                 body: "Something is wrong with ${env.BUILD_URL}"
        }
    }
}
