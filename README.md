obsi
====

obsi observes a directory and reports changes to your script.

it is written in c++ and uses inotify

build obsi
----------

  $ make

run
---

  $ obsi ./example.sh ./folder1

start watching directory ./folder1, changes are reported to the script ./example.sh

