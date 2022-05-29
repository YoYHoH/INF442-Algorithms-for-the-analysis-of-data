import sys


if __name__=="__main__":
	if len(sys.argv) >= 3:
		readfrom = sys.argv[1]
		writeto = sys.argv[2]
	else:
		print("Syntax: python %s <file_to_read> <file_to_write> [<id_label> <rest_of_cols> ['<char_sep>'] ]" % sys.argv[0])
		print(" <rest_of_cols> : columns to include in output file (ex: 1,2,3), starting in 1. Use - for including all.")
		exit(0)

	if len(sys.argv) >= 5:
		label = int(sys.argv[3])
		rest = sys.argv[4]
		if len(sys.argv) >= 6:
			char_sep = sys.argv[5]
		else:
			char_sep = ' '
	else:
		label = 1
		rest = "-"
		char_sep = ' '

	if rest != "-":
		some_cols = True
		id_cols = rest.rstrip().split(",")
	else:
		some_cols = False

	g = open(writeto, 'w')

	print("Character separation: [%c]"%char_sep)

	with open(readfrom, "r") as f:
		for line in f:
			cols = line.rstrip().split(char_sep)
			if cols[0].find("#") != -1 :
				continue
			if label > len(cols) :
				print("ERROR, indicated column for label does not exist (%d columns detected in file)"% len(cols))
			the_label = cols[label - 1]
			if the_label in ['0', '1']:  # switch 0 to -1
			    the_label = '-1' * (the_label == '0') + '1' * (the_label == '1')
			cad='%s ' % (the_label)
			index = 1
			ind_w = 1
			for item in cols:
				if (some_cols and not (str(index) in id_cols)):
					add = ""
				elif (index==label):
					add = ""
				elif index != label:
					add = "%d:%s " % (ind_w,item)
					ind_w += 1				
				
				index += 1

				if index - 1 == 1:  # 1st col is nbr line
					continue
				if '\n' in item:
					raw_input("ERROR: There should be no \n in item")
				try:
					if float(item) == 0.:
						continue
				except ValueError:
					continue
				cad += add
			g.write(cad + "\n")
		f.close()
	g.close()

