#include <stddef.h>
#include "limine.h"
#include "graphics/framebuffer.h"
#include "terminal/terminal.h"
#include "lib/num.h"
#include "gdt/gdt.h"
#include "debug.h"

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    fb = framebuffer_request.response->framebuffers[0]; 

    terminalInit();
    kprintf("Initiliazed terminal, yay! Test: %d | Test 2: 0x%x\n", 30, 0xFF00FF);
    
    // Load GDT
    kprintf("Loading GDT...\n");
    struct GDTDescriptor* gdtDescriptor = gdt_init(&defaultGDT);
    kprintf("If you are reading this, we did load the GDT! hooray! GDTDescriptor Address: %x\n", &gdtDescriptor);

    // We're done, just hang...
    hcf();
}
