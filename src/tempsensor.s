# tempsensor.s
# temperature sensor driver

# some assembler directives
.syntax unified
.cpu cortex-m4
.thumb

.section .data

# default sensor values for conversion
sensorVoltVal: .word 76000
sensorScaleVal: .word 25

.global Temp_Init
.global Temp_MeasureRaw
.global Temp_CelsiusFromRaw
.global ADC1_IRQ_Enable
.global ADC_IRQHandler
.global Temp_SelectExternal
.global Temp_SelectChip

# constants
.set RCC_BASE, 0x40023800
.set AHB1ENR_OFFSET, 0x30
.set APB2ENR_OFFSET, 0x44
.set GPIOA_EN, 0b1
.set ADC1_EN, 1<<8
.set MODER_OFFSET, 0x0
.set MODER6_MASK, (0b11 << 12)
.set GPIOA_BASE, 0x40020000
.set ADC1_BASE, 0x40012000+0x0
.set CCR_OFFSET, 0x04+0x300
.set CR1_OFFSET, 0x04
.set CR2_OFFSET, 0x08
.set SQR3_OFFSET, 0x34
.set SMPR1_OFFSET, 0x0C
.set SWSTART_MASK, 1<<30
.set SR_OFFSET, 0x0
.set EOC_MASK, 1<<1
.set ADCPRE_MASK_4, 1<<16
.set ADON_MASK, 1<<0
.set SQ1_POS, 0
.set SQ2_POS, 5
.set SMP18_POS, 24
.set RES_POS, 24
.set TSVREFE_MASK, 1<<23
.set VBATE_MASK, 1<<22
.set EOCIE, 1<<5
.set DR_OFFSET, 0x4C
.set NVIC_ISER0, 0xE000E100+0x0

.section .text

.thumb_func
ADC_IRQHandler:
	#r0-r3 autostacked

	ldr r0, =ADC1_BASE

	# read conversion result (clears interrupt flag)
	ldrh r1,[r0,DR_OFFSET]

	# save to memory
	ldr r2,=ADC1_Result
	str r1,[r2]

	# start new conversion
	ldr r1,[r0,CR2_OFFSET]
	orr r1,r1,SWSTART_MASK
	str r1,[r0,CR2_OFFSET]

	bx lr

ADC1_IRQ_Enable:
	push {r4,r5,lr}

	ldr r4, =ADC1_BASE
	# enable interrupt at end of conversion
	ldr r5,[r4,CR1_OFFSET]
	orr r5,r5,EOCIE
	str r5,[r4,CR1_OFFSET]

	# start ADC1 conversion
	ldr r5,[r4,CR2_OFFSET]
	orr r5,r5,SWSTART_MASK
	str r5,[r4,CR2_OFFSET]

	# enable IRQ 18 in NVIC
	ldr r4,=NVIC_ISER0
	ldr r5,[r4]
	orr r5,r5,(1<<18)
	str r5,[r4]

	pop {r4,r5,pc}

# temp_init
# PA6 with AF ADC12_IN6 for external sensor TMP36
# V25 = 750mV, Slope = 10mV/C, turn-on time(max) = 1ms
#
# ADC1_IN18 for internal sensor
# V25 = 760mV, Avg_Slope = 2.5mV/C, startup time = 10us, sampling time > 10us
Temp_Init:
	push {r4,lr}

	# enable AHB1 clock for GPIOA
	ldr r4, =RCC_BASE
	ldr r1,[r4,AHB1ENR_OFFSET]
	orr r1,r1,GPIOA_EN
	str r1,[r4,AHB1ENR_OFFSET]

	# enable APB2 clock for ADC1
	ldr r4, =RCC_BASE
	ldr r1,[r4,APB2ENR_OFFSET]
	orr r1,r1,ADC1_EN
	str r1,[r4,APB2ENR_OFFSET]

	# enable analog mode on PA6
	ldr r4, =GPIOA_BASE
	ldr r1,[r4,MODER_OFFSET]
	orr r1,r1,MODER6_MASK
	str r1,[r4,MODER_OFFSET]

	ldr r4, =ADC1_BASE

	# enable integrated temp sensor channel and set prescale to 4
	ldr r1,[r4,CCR_OFFSET]
	orr r1,r1,TSVREFE_MASK | ADCPRE_MASK_4
	bic r1,r1,VBATE_MASK
	str r1,[r4,CCR_OFFSET]

	# enable ADC1
	ldr r1,[r4,CR2_OFFSET]
	orr r1,r1,ADON_MASK
	str r1,[r4,CR2_OFFSET]

	# wait for ADC and temp sensor to startup
	mov r0,1
	bl System_DelayMs

	# select channel 18 (internal default)
	ldr r1,[r4,SQR3_OFFSET]
	orr r1,r1,(18<<SQ1_POS)
	str r1,[r4,SQR3_OFFSET]

	# 56 cycles sampling time
	ldr r1,[r4,SMPR1_OFFSET]
	orr r1,r1,(3<<SMP18_POS)
	str r1,[r4,SMPR1_OFFSET]

	# max resolution
	ldr r1,[r4,CR1_OFFSET]
	bic r1,r1,(3<<RES_POS)
	str r1,[r4,CR1_OFFSET]

	cmp r0,0
	beq 1f
	bl ADC1_IRQ_Enable

1:  pop {r4,pc}

Temp_SelectExternal:
	ldr r0, =ADC1_BASE
	# select channel 6 (external)
	ldr r1,[r0,SQR3_OFFSET]
	mov r1,(6<<SQ1_POS)
	str r1,[r0,SQR3_OFFSET]

	# update sensor info
	ldr r2, =sensorScaleVal
	mov r1,100
	str r1,[r2]
	ldr r2, =sensorVoltVal
	ldr r1,=75000
	str r1,[r2]

	# start new conversion
	ldr r1,[r0,CR2_OFFSET]
	orr r1,r1,SWSTART_MASK
	str r1,[r0,CR2_OFFSET]

	bx lr

Temp_SelectChip:
	ldr r0, =ADC1_BASE
	# select channel 18 (internal)
	ldr r1,[r0,SQR3_OFFSET]
	mov r1,(18<<SQ1_POS)
	str r1,[r0,SQR3_OFFSET]

	# update sensor info
	ldr r2,=sensorScaleVal
	mov r1,25
	str r1,[r2]
	ldr r2,=sensorVoltVal
	ldr r1,=76000
	str r1,[r2]

	# start new conversion
	ldr r1,[r0,CR2_OFFSET]
	orr r1,r1,SWSTART_MASK
	str r1,[r0,CR2_OFFSET]

	bx lr

#return raw temp
Temp_MeasureRaw:
	push {lr}

	ldr r0, =ADC1_BASE

	# start ADC conversion
	ldr r1,[r0,CR2_OFFSET]
	orr r1,r1,SWSTART_MASK
	str r1,[r0,CR2_OFFSET]

	# wait until done
1:	ldr r1,[r0,SR_OFFSET]
	tst r1,EOC_MASK
	beq 1b

	# read conversion result
	ldrh r0,[r0,DR_OFFSET]

	pop {pc}


#input: r0
#return: r0
Temp_CelsiusFromRaw:
	push {lr}

	# calculate temp
	ldr r1,=330000 //3.3V * 10000
	mul r0,r0,r1

	lsr r0,r0,12 // div by 2^12

	ldr r2,=sensorVoltVal //0.760 * 100000
	ldr r1,[r2]
	sub r0,r0,r1

	ldr r2,=sensorScaleVal // 0.025 * 10000
	ldr r1,[r2]
	sdiv r0,r0,r1

	add r0,r0,250

	pop {pc}
