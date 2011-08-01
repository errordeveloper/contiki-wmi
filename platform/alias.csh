## This relies on the fact that column 1 and 2 are okay.

## may be just $cwd or `pwd` will work equally well ??

if ( $?CONTIKI_CODE_PATH ) then
  set w=`find ${CONTIKI_CODE_PATH} -name 'alias'`
  if ( $#w != "1" ) then
    echo "More then one alias files found."
    ## grep "## platform alias" ??
    ## or try to detect the format ..??
    exit -1
  endif
else
  set w=($_)
  echo ${_[2]}
  set w=`dirname $w[2]`/alias
endif

echo $w

eval `awk '\
	$1 !~ /^#/ { \
	if ( $1 != "" && $2 != "" ) { \
		if ( $3 == "" ) { \
        		 print ( "alias make."$1" \"make TARGET="$2"\";" ) \
		} else { print ( "alias make."$1" \"make TARGET="$2" UDEV_UART="$3"\";" ) \
    } } }' $w`

unset w
