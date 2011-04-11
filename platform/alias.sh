## This relies on the fact that column 1 and 2 are okay.
w=`dirname $BASH_SOURCE`
eval `awk '\
	$1 !~ /^#/ { \
	if ( $1 != "" && $2 != "" ) { \
		if ( $3 == "" ) { \
			 print ( "alias make."$1"=\"make TARGET="$2"\";" ) \
		} else { print ( "alias make."$1"=\"make TARGET="$2" UDEV_UART="$3"\";" ) \
    } } }' $w/alias`
