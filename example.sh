#!/bin/bash

shopt -s extglob

IN_ACCESS=$((0x00000001))        # File was accessed
IN_MODIFY=$((0x00000002))        # File was modified
IN_ATTRIB=$((0x00000004))        # Metadata changed
IN_CLOSE_WRITE=$((0x00000008))   # Writtable file was closed
IN_CLOSE_NOWRITE=$((0x00000010)) # Unwrittable file closed
IN_OPEN=$((0x00000020))          # File was opened
IN_MOVED_FROM=$((0x00000040))    # File was moved from X
IN_MOVED_TO=$((0x00000080))      # File was moved to Y
IN_CREATE=$((0x00000100))        # Subfile was created
IN_DELETE=$((0x00000200))        # Subfile was deleted
IN_DELETE_SELF=$((0x00000400))   # Self was deleted
IN_MOVE_SELF=$((0x00000800))     # Self was moved

echo example.sh called with $@

[ $(( $1 & $IN_ACCESS )) -gt 0 ]        && echo "got IN_ACCESS          $2"
[ $(( $1 & $IN_MODIFY )) -gt 0 ]        && echo "got IN_MODIFY 		$2"
[ $(( $1 & $IN_ATTRIB )) -gt 0 ]        && echo "got IN_ATTRIB 		$2"
[ $(( $1 & $IN_CLOSE_WRITE )) -gt 0 ]   && echo "got IN_CLOSE_WRITE     $2"
[ $(( $1 & $IN_CLOSE_NOWRITE )) -gt 0 ] && echo "got IN_CLOSE_NOWRITE   $2"
[ $(( $1 & $IN_OPEN )) -gt 0 ]          && echo "got IN_OPEN 		$2"
[ $(( $1 & $IN_MOVED_FROM )) -gt 0 ]    && echo "got IN_MOVED_FROM    	$2"
[ $(( $1 & $IN_MOVED_TO )) -gt 0 ]      && echo "got IN_MOVED_TO 	$2"
[ $(( $1 & $IN_CREATE )) -gt 0 ]        && echo "got IN_CREATE 		$2"
[ $(( $1 & $IN_DELETE )) -gt 0 ]        && echo "got IN_DELETE 		$2"
[ $(( $1 & $IN_DELETE_SELF )) -gt 0 ]   && echo "got IN_DELETE_SELF 	$2"
[ $(( $1 & $IN_MOVE_SELF )) -gt 0 ]     && echo "got IN_MOVE_SELF 	$2"
exit 0
