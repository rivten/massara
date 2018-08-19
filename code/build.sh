pushd "../build"

yasm -f elf32 ../code/kernel.asm -o kasm.o
clang -m32 -c ../code/kernel.c -o kc.o
ld -m elf_i386 -T ../code/link.ld -o kernel-massara kasm.o kc.o

popd
