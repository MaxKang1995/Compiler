	.data

s0:	.asciiz "arrI set value from "
s1:	.asciiz " to "
s2:	.asciiz " "
s3:	.asciiz "number1 = number2   "
s4:	.asciiz "number1 <= number2   "
s5:	.asciiz "number1 >= number2   "
s6:	.asciiz "number1 <> number2   "
s7:	.asciiz " "
s8:	.asciiz " "
s9:	.asciiz " "
s10:	.asciiz " "
s11:	.asciiz "initializing   @@@   "
s12:	.asciiz "const CON1 = "
s13:	.asciiz "   CON2 = "
s14:	.asciiz "   CON3 = "
s15:	.asciiz "   CON4 = "
s16:	.asciiz "   @@@   initialization finished!   @@@   "
s17:	.asciiz "function 1   @@@   "
s18:	.asciiz "result : "
s19:	.asciiz "   @@@   "
s20:	.asciiz "function 2   @@@   "
s21:	.asciiz "number1 : "
s22:	.asciiz "   number2 : "
s23:	.asciiz "   @@@   "
s24:	.asciiz "function 3   @@@   "
s25:	.asciiz "input number between 1 and 20 : "
s26:	.asciiz "   @@@   "
s27:	.asciiz "over number bound [1, 20]"
s28:	.asciiz "over number bound [1, 20]"
s29:	.asciiz "the fibonacci number is : "
s30:	.asciiz "   @@@   "
s31:	.asciiz "function 4   @@@   "
s32:	.asciiz "number3 : "
s33:	.asciiz "number4(1|2|3) : "
s34:	.asciiz "wrong type"
s35:	.asciiz "wrong type"
s36:	.asciiz "   @@@   "
s37:	.asciiz "END"

	.text
li	$fp, 644
li	$sp, 644
li	$t0, 644
addi	$sp, $sp, 144
j	entry
# LABEL label_1 #
label_1:
# ASG   ii, 0 #
li	$t0, 0
sw	$t0, 16($fp)
# LABEL label_2 #
label_2:
# SUB   #0, max, 1 #
li	$t0, 10
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 20($fp)
# BGT   label_3, ii, #0 #
lw	$t0, 16($fp)
lw	$t1, 20($fp)
bgt	$t0, $t1, label_3
# OFF   #1, arri, ii #
lw	$t2, 8($fp)
la	$t0, 16($t2)
lw	$t1, 16($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
sw	$t0, 24($fp)
# ASG   #1, m #
lw	$t0, -4($fp)
lw	$t2, 24($fp)
sw	$t0, 0($t2)
# ADD   #2, m, 1 #
lw	$t0, -4($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 28($fp)
# ASG   m, #2 #
lw	$t0, 28($fp)
sw	$t0, -4($fp)
# ADD   ii, ii, 1 #
lw	$t0, 16($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 16($fp)
# JMP   label_2 #
j	label_2
# LABEL label_3 #
label_3:
# WRITE "arrI set value from " #
li	$v0, 4
la	$a0, s0
syscall
# SUB   #3, m, 10 #
lw	$t0, -4($fp)
li	$t1, 10
sub	$t0, $t0, $t1
sw	$t0, 32($fp)
# WRITE #3 #
li	$v0, 1
lw	$a0, 32($fp)
syscall
# WRITE " to " #
li	$v0, 4
la	$a0, s1
syscall
# WRITE m #
li	$v0, 1
lw	$a0, -4($fp)
syscall
# RET   setarri #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 4
jr	$ra
# LABEL label_4 #
label_4:
# ASG   ii, 0 #
li	$t0, 0
sw	$t0, 16($fp)
# LABEL label_5 #
label_5:
# SUB   #4, max, 1 #
li	$t0, 10
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 20($fp)
# BGT   label_6, ii, #4 #
lw	$t0, 16($fp)
lw	$t1, 20($fp)
bgt	$t0, $t1, label_6
# OFF   #5, arri, ii #
lw	$t2, 8($fp)
la	$t0, 16($t2)
lw	$t1, 16($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
sw	$t0, 24($fp)
# ASG   #5, m #
lw	$t2, 12($fp)
lw	$t0, -8($t2)
lw	$t2, 24($fp)
sw	$t0, 0($t2)
# ADD   ii, ii, 1 #
lw	$t0, 16($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 16($fp)
# JMP   label_5 #
j	label_5
# LABEL label_6 #
label_6:
# ASG   ii, 0 #
li	$t0, 0
sw	$t0, 16($fp)
# LABEL label_7 #
label_7:
# SUB   #6, max, 1 #
li	$t0, 10
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 28($fp)
# BGT   label_8, ii, #6 #
lw	$t0, 16($fp)
lw	$t1, 28($fp)
bgt	$t0, $t1, label_8
# WRITE " " #
li	$v0, 4
la	$a0, s2
syscall
# SEL   #7, arri, ii #
lw	$t2, 8($fp)
la	$t0, 16($t2)
lw	$t1, 16($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
lw	$t0, 0($t0)
sw	$t0, 32($fp)
# WRITE #7 #
li	$v0, 1
lw	$a0, 32($fp)
syscall
# ADD   ii, ii, 1 #
lw	$t0, 16($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 16($fp)
# JMP   label_7 #
j	label_7
# LABEL label_8 #
label_8:
# RET   getarri #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 0
jr	$ra
# LABEL label_9 #
label_9:
# ASG   ii, 0 #
li	$t0, 0
sw	$t0, 16($fp)
# LABEL label_10 #
label_10:
# SUB   #8, max, 1 #
li	$t0, 10
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 20($fp)
# BGT   label_11, ii, #8 #
lw	$t0, 16($fp)
lw	$t1, 20($fp)
bgt	$t0, $t1, label_11
# SEL   #9, arrc, ii #
lw	$t2, 8($fp)
la	$t0, 56($t2)
lw	$t1, 16($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
lw	$t0, 0($t0)
sw	$t0, 24($fp)
# WRITE #9 #
li	$v0, 11
lw	$a0, 24($fp)
syscall
# ADD   ii, ii, 1 #
lw	$t0, 16($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 16($fp)
# JMP   label_10 #
j	label_10
# LABEL label_11 #
label_11:
# RET   printarrc #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 0
jr	$ra
# LABEL label_0 #
label_0:
# BNE   label_13, t, 1 #
lw	$t0, -4($fp)
li	$t1, 1
bne	$t0, $t1, label_13
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSH  setarri, 1, m #
lw	$t0, -8($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALP  label_1 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
lw	$t1, 8($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_1
lw	$31, -4($sp)
subi	$sp, $sp, 4
# JMP   label_12 #
j	label_12
# LABEL label_13 #
label_13:
# BNE   label_14, t, 2 #
lw	$t0, -4($fp)
li	$t1, 2
bne	$t0, $t1, label_14
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# CALP  label_4 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
lw	$t1, 8($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_4
lw	$31, -4($sp)
subi	$sp, $sp, 4
# JMP   label_12 #
j	label_12
# LABEL label_14 #
label_14:
# BNE   label_15, t, 3 #
lw	$t0, -4($fp)
li	$t1, 3
bne	$t0, $t1, label_15
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# CALP  label_9 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
lw	$t1, 8($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 12
jal	label_9
lw	$31, -4($sp)
subi	$sp, $sp, 4
# JMP   label_12 #
j	label_12
# LABEL label_15 #
label_15:
# LABEL label_12 #
label_12:
# RET   sequence #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 8
jr	$ra
# LABEL label_16 #
label_16:
# BNE   label_17, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
bne	$t0, $t1, label_17
# WRITE "number1 = number2   " #
li	$v0, 4
la	$a0, s3
syscall
# LABEL label_17 #
label_17:
# BGT   label_18, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
bgt	$t0, $t1, label_18
# WRITE "number1 <= number2   " #
li	$v0, 4
la	$a0, s4
syscall
# LABEL label_18 #
label_18:
# BLT   label_19, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
blt	$t0, $t1, label_19
# WRITE "number1 >= number2   " #
li	$v0, 4
la	$a0, s5
syscall
# LABEL label_19 #
label_19:
# BEQ   label_20, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
beq	$t0, $t1, label_20
# WRITE "number1 <> number2   " #
li	$v0, 4
la	$a0, s6
syscall
# LABEL label_20 #
label_20:
# WRITE " " #
li	$v0, 4
la	$a0, s7
syscall
# ADD   #10, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
add	$t0, $t0, $t1
sw	$t0, 16($fp)
# WRITE #10 #
li	$v0, 1
lw	$a0, 16($fp)
syscall
# WRITE " " #
li	$v0, 4
la	$a0, s8
syscall
# SUB   #11, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
sub	$t0, $t0, $t1
sw	$t0, 20($fp)
# WRITE #11 #
li	$v0, 1
lw	$a0, 20($fp)
syscall
# WRITE " " #
li	$v0, 4
la	$a0, s9
syscall
# MUL   #12, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
mul	$t0, $t0, $t1
sw	$t0, 24($fp)
# WRITE #12 #
li	$v0, 1
lw	$a0, 24($fp)
syscall
# BEQ   label_21, n, 0 #
lw	$t2, -4($fp)
lw	$t0, 0($t2)
li	$t1, 0
beq	$t0, $t1, label_21
# WRITE " " #
li	$v0, 4
la	$a0, s10
syscall
# DIV   #13, m, n #
lw	$t2, -8($fp)
lw	$t0, 0($t2)
lw	$t2, -4($fp)
lw	$t1, 0($t2)
div	$t0, $t0, $t1
sw	$t0, 28($fp)
# WRITE #13 #
li	$v0, 1
lw	$a0, 28($fp)
syscall
# LABEL label_21 #
label_21:
# RET   calculate #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 8
jr	$ra
# LABEL label_22 #
label_22:
# WRITE "initializing   @@@   " #
li	$v0, 4
la	$a0, s11
syscall
# ASG   a, 0 #
li	$t0, 0
lw	$t2, 8($fp)
sw	$t0, 0($t2)
# ASG   b, 0 #
li	$t0, 0
lw	$t2, 8($fp)
sw	$t0, 4($t2)
# ASG   c, con4 #
li	$t0, 48
lw	$t2, 8($fp)
sw	$t0, 8($t2)
# ASG   d, con4 #
li	$t0, 48
lw	$t2, 8($fp)
sw	$t0, 12($t2)
# SUB   #14, max, 1 #
li	$t0, 10
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 16($fp)
# ASG   ii, #14 #
lw	$t0, 16($fp)
sw	$t0, 12($fp)
# LABEL label_23 #
label_23:
# BLT   label_24, ii, 0 #
lw	$t0, 12($fp)
li	$t1, 0
blt	$t0, $t1, label_24
# OFF   #15, arri, ii #
lw	$t2, 8($fp)
la	$t0, 16($t2)
lw	$t1, 12($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
sw	$t0, 20($fp)
# ASG   #15, 1 #
li	$t0, 1
lw	$t2, 20($fp)
sw	$t0, 0($t2)
# OFF   #16, arrc, ii #
lw	$t2, 8($fp)
la	$t0, 56($t2)
lw	$t1, 12($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
sw	$t0, 24($fp)
# ASG   #16, con3 #
li	$t0, 97
lw	$t2, 24($fp)
sw	$t0, 0($t2)
# ADD   ii, ii, -1 #
lw	$t0, 12($fp)
li	$t1, -1
add	$t0, $t0, $t1
sw	$t0, 12($fp)
# JMP   label_23 #
j	label_23
# LABEL label_24 #
label_24:
# WRITE "const CON1 = " #
li	$v0, 4
la	$a0, s12
syscall
# WRITE con1 #
li	$v0, 1
li	$a0, 1
syscall
# WRITE "   CON2 = " #
li	$v0, 4
la	$a0, s13
syscall
# WRITE con2 #
li	$v0, 1
li	$a0, -2
syscall
# WRITE "   CON3 = " #
li	$v0, 4
la	$a0, s14
syscall
# WRITE con3 #
li	$v0, 11
li	$a0, 97
syscall
# WRITE "   CON4 = " #
li	$v0, 4
la	$a0, s15
syscall
# WRITE con4 #
li	$v0, 11
li	$a0, 48
syscall
# WRITE "   @@@   initialization finished!   @@@   " #
li	$v0, 4
la	$a0, s16
syscall
# RET   initiate #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 0
jr	$ra
# LABEL label_25 #
label_25:
# BNE   label_26, i, 1 #
lw	$t0, -4($fp)
li	$t1, 1
bne	$t0, $t1, label_26
# RVAL  1 #
li	$t0, 1
sw	$t0, 0($fp)
# JMP   label_27 #
j	label_27
# LABEL label_26 #
label_26:
# BNE   label_28, i, 2 #
lw	$t0, -4($fp)
li	$t1, 2
bne	$t0, $t1, label_28
# RVAL  1 #
li	$t0, 1
sw	$t0, 0($fp)
# JMP   label_29 #
j	label_29
# LABEL label_28 #
label_28:
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# SUB   #17, i, 1 #
lw	$t0, -4($fp)
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 12($fp)
# PUSH  fibonacci, 1, #17 #
lw	$t0, 12($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALF  label_25, #18 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
lw	$t1, 8($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_25
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 16($fp)
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# SUB   #19, i, 2 #
lw	$t0, -4($fp)
li	$t1, 2
sub	$t0, $t0, $t1
sw	$t0, 20($fp)
# PUSH  fibonacci, 1, #19 #
lw	$t0, 20($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALF  label_25, #20 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
lw	$t1, 8($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_25
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 24($fp)
# ADD   #21, #18, #20 #
lw	$t0, 16($fp)
lw	$t1, 24($fp)
add	$t0, $t0, $t1
sw	$t0, 28($fp)
# RVAL  #21 #
lw	$t0, 28($fp)
sw	$t0, 0($fp)
# LABEL label_29 #
label_29:
# LABEL label_27 #
label_27:
# RET   fibonacci #
move	$sp, $fp
lw	$t0, 4($fp)
lw	$v0, 0($fp)
move	$fp, $t0
sub	$sp, $sp, 4
jr	$ra
# LABEL entry #
entry:
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# CALP  label_22 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 16
jal	label_22
lw	$31, -4($sp)
subi	$sp, $sp, 4
# WRITE "function 1   @@@   " #
li	$v0, 4
la	$a0, s17
syscall
# WRITE "result : " #
li	$v0, 4
la	$a0, s18
syscall
# ADD   #22, a, 1 #
lw	$t0, 0($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 96($fp)
# SEL   #23, arri, #22 #
la	$t0, 16($fp)
lw	$t1, 96($fp)
mul	$t1, $t1, 4
add	$t0, $t0, $t1
lw	$t0, 0($t0)
sw	$t0, 100($fp)
# NEG   #24, con3 #
li	$t0, 97
neg	$t0, $t0
sw	$t0, 104($fp)
# ADD   #25, con3, 1 #
li	$t0, 97
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 108($fp)
# ADD   #26, #24, #25 #
lw	$t0, 104($fp)
lw	$t1, 108($fp)
add	$t0, $t0, $t1
sw	$t0, 112($fp)
# MUL   #27, #23, #26 #
lw	$t0, 100($fp)
lw	$t1, 112($fp)
mul	$t0, $t0, $t1
sw	$t0, 116($fp)
# SEL   #28, arri, 9 #
la	$t0, 16($fp)
li	$t1, 9
mul	$t1, $t1, 4
add	$t0, $t0, $t1
lw	$t0, 0($t0)
sw	$t0, 120($fp)
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSH  fibonacci, 1, 2 #
li	$t0, 2
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALF  label_25, #29 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_25
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 124($fp)
# NEG   #30, #29 #
lw	$t0, 124($fp)
neg	$t0, $t0
sw	$t0, 128($fp)
# DIV   #31, #28, #30 #
lw	$t0, 120($fp)
lw	$t1, 128($fp)
div	$t0, $t0, $t1
sw	$t0, 132($fp)
# SUB   #32, #27, #31 #
lw	$t0, 116($fp)
lw	$t1, 132($fp)
sub	$t0, $t0, $t1
sw	$t0, 136($fp)
# WRITE #32 #
li	$v0, 1
lw	$a0, 136($fp)
syscall
# WRITE "   @@@   " #
li	$v0, 4
la	$a0, s19
syscall
# WRITE "function 2   @@@   " #
li	$v0, 4
la	$a0, s20
syscall
# WRITE "number1 : " #
li	$v0, 4
la	$a0, s21
syscall
# READ  a #
li	$v0, 5
syscall
sw	$v0, 0($fp)
# WRITE "   number2 : " #
li	$v0, 4
la	$a0, s22
syscall
# READ  b #
li	$v0, 5
syscall
sw	$v0, 4($fp)
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSH  calculate, 1, a #
la	$t0, 0($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# PUSH  calculate, 2, b #
la	$t0, 4($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALP  label_16 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_16
lw	$31, -4($sp)
subi	$sp, $sp, 4
# WRITE "   @@@   " #
li	$v0, 4
la	$a0, s23
syscall
# WRITE "function 3   @@@   " #
li	$v0, 4
la	$a0, s24
syscall
# WRITE "input number between 1 and 20 : " #
li	$v0, 4
la	$a0, s25
syscall
# READ  a #
li	$v0, 5
syscall
sw	$v0, 0($fp)
# WRITE "   @@@   " #
li	$v0, 4
la	$a0, s26
syscall
# BLE   label_30, a, 20 #
lw	$t0, 0($fp)
li	$t1, 20
ble	$t0, $t1, label_30
# WRITE "over number bound [1, 20]" #
li	$v0, 4
la	$a0, s27
syscall
# JMP   label_31 #
j	label_31
# LABEL label_30 #
label_30:
# BGE   label_32, a, 1 #
lw	$t0, 0($fp)
li	$t1, 1
bge	$t0, $t1, label_32
# WRITE "over number bound [1, 20]" #
li	$v0, 4
la	$a0, s28
syscall
# JMP   label_33 #
j	label_33
# LABEL label_32 #
label_32:
# WRITE "the fibonacci number is : " #
li	$v0, 4
la	$a0, s29
syscall
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSH  fibonacci, 1, a #
lw	$t0, 0($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALF  label_25, #33 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 20
jal	label_25
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 140($fp)
# WRITE #33 #
li	$v0, 1
lw	$a0, 140($fp)
syscall
# LABEL label_33 #
label_33:
# LABEL label_31 #
label_31:
# WRITE "   @@@   " #
li	$v0, 4
la	$a0, s30
syscall
# WRITE "function 4   @@@   " #
li	$v0, 4
la	$a0, s31
syscall
# WRITE "number3 : " #
li	$v0, 4
la	$a0, s32
syscall
# READ  a #
li	$v0, 5
syscall
sw	$v0, 0($fp)
# WRITE "number4(1|2|3) : " #
li	$v0, 4
la	$a0, s33
syscall
# READ  b #
li	$v0, 5
syscall
sw	$v0, 4($fp)
# BGE   label_34, b, 1 #
lw	$t0, 4($fp)
li	$t1, 1
bge	$t0, $t1, label_34
# WRITE "wrong type" #
li	$v0, 4
la	$a0, s34
syscall
# JMP   label_35 #
j	label_35
# LABEL label_34 #
label_34:
# BLE   label_36, b, 3 #
lw	$t0, 4($fp)
li	$t1, 3
ble	$t0, $t1, label_36
# WRITE "wrong type" #
li	$v0, 4
la	$a0, s35
syscall
# JMP   label_37 #
j	label_37
# LABEL label_36 #
label_36:
# SAVE   #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSH  sequence, 1, a #
lw	$t0, 0($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# PUSH  sequence, 2, b #
lw	$t0, 4($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALP  label_0 #
move	$t0, $fp
move	$fp, $sp
addi	$sp, $sp, 4
addi	$sp, $sp, 4
sw	$t0, -4($sp)
# copy SLs from previous frame #
# append new SL #
sw	$t0, 0($sp)
addi	$sp, $sp, 4
addi	$sp, $sp, 0
jal	label_0
lw	$31, -4($sp)
subi	$sp, $sp, 4
# LABEL label_37 #
label_37:
# LABEL label_35 #
label_35:
# WRITE "   @@@   " #
li	$v0, 4
la	$a0, s36
syscall
# WRITE "END" #
li	$v0, 4
la	$a0, s37
syscall
