#!/bin/bash

images=(img/lena img/room img/killeroos img/plants img/sanmiguel)
names=(lena room killeroos plants sanmiguel)
ratios=(40 60 80)
trans=(2 3 4)
transname=(DCT DB8)
blocks=(16 32)

for i in `seq ${#images[*]}`; do
	img=${images[$i-1]}
	name=${names[$i-1]}
	for r in `seq ${#ratios[*]}`; do
		ra=${ratios[$r-1]}
		./GEN ${img}.ppm 0.${ra}
		for t in `seq ${#trans[*]}`; do
			tr=${trans[$t-1]}
			trname=${transname[$t-1]}
			for b in `seq ${#blocks[*]}`; do
				bs=${blocks[$b-1]}
				echo $name $ra $tr $bs
				direct=test/${name}-${ra}-${trname}-${bs}
				mkdir $direct
				/usr/bin/time -o $direct/time ./CSCLI ${img}H${ra}.ppm 0.01 ${bs} ${tr} 2> /dev/null > /dev/null
				mv 0-original.ppm $direct
				mv 1-transformed.ppm $direct
				mv 2-reconstructed.ppm $direct
			done
		done;
	done;
done;

