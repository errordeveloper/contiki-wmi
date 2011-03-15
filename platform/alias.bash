## This relies on the fact that column 1 and 2 are okay.
## Tanks to _ivan_ at #gentoo-ru on irc.freenode.net
while read key value
do
	eval "make.$key() { make TARGET=\"$value\"; }"
	done < <( egrep -v '^(#|$)' `dirname $BASH_SOURCE`/alias )
