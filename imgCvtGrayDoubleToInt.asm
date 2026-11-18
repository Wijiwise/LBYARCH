; ecx = int height
; edx = int width
; r8 = const double *input
; r9 = uint8_t *output

section .data
scale dq 255.0
round dq 0.5

section .text
default rel
bits 64
global imgCvtGrayDoubleToInt
imgCvtGrayDoubleToInt:
  imul ecx, edx  ; total pixel count = height * width
  cmp ecx, 0
  jle FINISH
  movsxd rcx, ecx
  xor r10, r10  ; i = 0
  movsd xmm1, [scale]
  movsd xmm2, [round]
  L1:
  movsd xmm0, [r8 + r10 * 8]  ; xmm0 = input[i]
  mulsd xmm0, xmm1  ; xmm0 = input[i] * 255.0
  addsd xmm0, xmm2  ; xmm0 = input[i] * 255.0 + 0.5
  cvttsd2si eax, xmm0
  mov [r9 + r10], al
  inc r10
  cmp r10, rcx
  jl L1
  FINISH:
  ret