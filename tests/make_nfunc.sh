FILEA="functions"
FILEB="functions"
FILEC="vars"
FILED="vars1"

while read f
do
while read v
do
while read j
do
while read i
do
echo "("$f "("$v $i "("$f "("$v $i $j")" $j $i")" $i ")" $j $i")"
done < $FILED
done < $FILEC
done < $FILEB
done < $FILEA

