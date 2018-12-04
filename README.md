# JACK_PLAYREC
jack_playrec provides an interface for synchronous recording/playback via the JACK Audio Connection Kit.
In addition to the cli a Matlab wrapper is provided.

## Installation
### Compiling from Source (Linux)

#### Prerequisites
- g++ (tested with g++-5 and g++-8)
- make
- libsndfile1-dev
- libjack-jackd2-dev
- jackd2

#### Compilation

After downloading and unpacking the jack_playrec tarball, or cloning from github,
compile openMHA with by typing in a terminal (while in the jack_playrec directory)
```
make
```

### Compiling from Source (other OSes)

Other operating systems are not officially supported, compilation has however been
reported to work. You may need to change the Makefile to adapt to your environment.

## Command line interface
Usage:
```
jack_playrec [options] input.wav [ ports [...]]
```

## Matlab wrapper
 Usage:
 ```
 [y,fs,bufsize,load,xruns,sCfg] = jack_playrec( x, ... );
```
 Examples:
 
 Display list of valid key/value pairs:
```
jack_playrec help
```
 Playback of MATLAB matrix x on hardware outputs (one column per channel):
 ```
 jack_playrec( x );
```
 Playback of MATLAB data via user defined ports:
 ```
 jack_playrec( x, 'output', csOutputPorts );
```
 Synchronouos playback and recording of MATLAB vector:
 ```
 [y,fs,bufsize] = jack_playrec( x, 'output', csOutputPorts, 'input', csInputPorts );
```
 Synchronouos playback and recording, with jack transport:
```
[y,fs,bufsize] = jack_playrec( x, 'input', csInputPorts, 'starttime', transportStart );
```
 Synchronouos playback and recording, in freewheeling mode:
 ```
 [y,fs,bufsize] = jack_playrec( x, 'input', csInputPorts, 'freewheeling', true );
```
 If csOutputPorts and csInputPorts are a single string, a single
 port name is used. If they are a cell string array, the number of
 channels in x has to match the number of elements in
 csOutputPorts, and the number of channels in y will match the
 number of channels in csInputPorts. If the argument is a numeric
 vector, then the physical ports (starting from 1) are used.
