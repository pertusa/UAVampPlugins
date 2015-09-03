# README #

## Description ##

UAPlugins are a set of VAMP plugins developed by the Computer Music
Laboratory team from the GRFIA group at the University of Alicante.

The methods used in this library were developed by Antonio Pertusa and José
Manuel Iñesta, supported by the project DRIMS (code
TIN2009-14247-C02), the Consolider Ingenio 2010 research programme (project
MIPRCV, CSD2007-00018), and the PASCAL2 Network of Excellence,
IST-2007-216886. 

## Dependencies ##

The VAMP plugin SDK requires libsndfile to be compiled. Besides, the
UAPlugins require boost and fftw3 libraries.

## Compilation ##

OSX: 
```
make -f Makefile.osx
```
Linux: 
```
make -f Makefile.linux
```
Windows (from Linux with mingw): 
```
make -f Makefile.mingw
```

## Algorithms ##

### Onset detection ###

The onset detection algorithm relies on a one-semitone filterbank to
detect abrupt energy changes in the harmonic contents of a signal. It was
evaluated in MIREX 09 and described in the following paper:

Pertusa, A., Iñesta, J.M.
"Note Onset Detection Using One Semitone Filter-Bank For MIREX 2009", MIREX
2009 - Music Information Retrieval Evaluation eXchange, MIREX Audio Onset
Detection, Kobe, Japan (2009)

### Multiple fundamental frequency estimation ###

The multiple fundamental frequency estimation algorithms are described in:

Pertusa, A., Iñesta, J.M.
"Efficient methods for joint estimation of multiple fundamental frequencies
in music signals"
EURASIP Journal on Advances in Signal Processing, vol. 2012, pp. 27 (2012) 

The algorithm called EURASIP refers to the extended method of section 3 from
this paper, which was evaluated in the MIREX 2008 contest with reference
PI2. The method EURASIP+tracking corresponds to the same method but with
tracking, as described in the same paper and already evaluated in MIREX 2008
as PI1.

For the Onset-based algorithm, combinations of those frames that are between two
consecutive onsets are merged to yield the pitches within the inter-onset
interval. This technique is close to segmentation, and it can obtain
reliable results when the onsets are correctly estimated, as it happens with
sharp attack sounds like piano.

These algorithms can be also be downloaded from
http://grfia.dlsi.ua.es/cm/projects/drims/software.php to be compiled and
executed without a VAMP host.

## More info ##

http://grfia.dlsi.ua.es/cm/projects/drims/softwareVAMP.php
