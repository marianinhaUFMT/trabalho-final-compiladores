section .text
soma:
push ebp
mov ebp, esp
mov eax, [ebp+8] ; a
push eax
mov eax, [ebp+12] ; b
mov ecx, eax
pop eax
add eax, ecx
mov esp, ebp
pop ebp
ret
global main
main:
push ebp
mov ebp, esp
sub esp, 4
mov eax, 3
push eax
mov eax, 5
push eax
call soma
add esp, 8
mov [ebp-4], eax ; x
mov eax, 0
mov esp, ebp
pop ebp
ret
