# Calculates Fibonacci numbers
#
# The program uses five cells
# which are named from a to e
# within the comments
#
# Cell a contains the counter
# Cell b contains F_n
# Cell c contains F_{n+1}
# Cell d and e are for temporary use

++++++++++			# put the number of fibonacci numbers to compute to cell a
>+>+<<			# put the initial values 1 and 1 to cells b and c
>.>.<<			# print the initial values
[			
-			# decrement the counter in cell a
>>			# move pointer to cell c
[->++>+<<]>>[-<-<+>>]<<	# copy the value of cell c to cell d (using cell e as temporary storage)
<			# move pointer back to cell b
[->+<]			# add value of cell b to cell c (leaving cell b with 0)
>>			# move pointer to cell d
[-<<+>>]			# shift content of cell d to cell b
<			# move pointer to cell c
.			# print content of cell c (the next fibonacci number)
<<			# move pointer to cell a
]

