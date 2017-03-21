FILEA="vars"
FILEB="vars"

while read f
do
	while read v
	do
		echo $f $v
	done < $FILEB
done < $FILEA
