// NOTE(hugo): IDT stands for
// Interrupt Descriptor Table
typedef struct
{
	unsigned short int OffsetLowerbits;
	unsigned short int Selector;
	unsigned char Zero;
	unsigned char TypeAttribute;
	unsigned short int OffsetHigherbits;
} idt_entry;

idt_entry IDT[IDT_SIZE];

void IDT_Init(void)
{
	unsigned long KeyboardAddress;
	unsigned long IDTAddress;
	unsigned long IDTPtr[2];

	KeyboardAddress = (unsigned long)KeyboardHandler;
	IDT[0x21].OffsetLowerbits = KeyboardAddress & 0xFFFF;
	IDT[0x21].Selector = 0x08; // NOTE(hugo): KERNEL_CODE_SEGMENT_OFFSET
	IDT[0x21].Zero = 0;
	IDT[0x21].TypeAttribute = 0x8E;
	IDT[0x21].OffsetHigherbits = (KeyboardAddress & 0xFFFF0000) >> 16;

	WritePort(0x20, 0x11);
	WritePort(0xA0, 0x11);

	WritePort(0x21, 0x20);
	WritePort(0xA1, 0x28);

	WritePort(0x21, 0x00);
	WritePort(0xA1, 0x00);

	WritePort(0x21, 0x01);
	WritePort(0xA1, 0x01);

	WritePort(0x21, 0xFF);
	WritePort(0xA1, 0xFF);

	IDTAddress = (unsigned long)IDT;
	IDTPtr[0] = (sizeof(idt_entry) * IDT_SIZE) + ((IDTAddress & 0xFFFF) << 16);
	IDTPtr[1] = IDTAddress >> 16;

	LoadIDT(IDTPtr);
}

void KeyboardInit(void)
{
	WritePort(0x21, 0xFD);
}

void KeyboardHandlerMain(void)
{
	unsigned char Status;
	char Keycode;

	WritePort(0x20, 0x20);
	Status = ReadPort(KEYBOARD_STATUS_PORT);

	if(Status & 0x01)
	{
		Keycode = ReadPort(KEYBOARD_DATA_PORT);
		if(Keycode < 0)
		{
			return;
		}
		VideoPtr[CurrentLocation] = KeyboardMap[Keycode];
		++CurrentLocation;
		VideoPtr[CurrentLocation] = 0x07;
		++CurrentLocation;
	}
}

void kernel_main(void)
{
	const char* Str = "Massara Kernel";
	char* VideoPtr = (char *)0xb8000;
	unsigned int i = 0;
	unsigned int j = 0;

	while(j < 80 * 25 * 2)
	{
		VideoPtr[j] = ' ';
		VideoPtr[j + 1] = 0x07;
		j = j + 2;
	}

	j = 0;

	while(Str[j] != '\0')
	{
		VideoPtr[i] = Str[j];
		VideoPtr[i + 1] = 0x07;
		++j;
		i = i + 2;
	}

	return;
}
