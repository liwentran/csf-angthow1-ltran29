/*
 * Assembly-language implementations of the drawing functions
 * CSF Assignment 2
 * Liwen Tran, Anthony Sky Ng-Thow-Hing
 * ltran29@jhu.edu, angthow1@jhu.edu
 */

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/* Offsets of struct Rect fields */
#define RECT_X_OFFSET        0
#define RECT_Y_OFFSET        4
#define RECT_WIDTH_OFFSET    8
#define RECT_HEIGHT_OFFSET   12

	.section .text

/***********************************************************************
   Helper functions
   You should declare these as .globl so that
   they can be called from the unit test code.
 ***********************************************************************/

/* TODO: implement your helper functions here */

/* Checks x and y coordinates to determine whether they are in bounds in the specified image.
 *
 * Parameters:
 *   %rdi - pointer to Image
 *   %esi - x coordinate (pixel column)
 *   %edx - y coordinate (pixel row)
 */
	.globl in_bounds
in_bounds:
	movl IMAGE_WIDTH_OFFSET(%rdi), %r10d	/* move pointer value of image width to temporary registrar %r10 */
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r11d	/* move pointer value of image width to temporary registrar %r11 */
	
	cmpl $0, %esi		/* Compare 0 with esi */
	jl .Lout_of_bounds	/* jump to Lout_of_bounds if esi is less than 0  */

	cmpl $0, %edx		/* Compare 0 with edx */
	jl .Lout_of_bounds 	/* jump to Lout_of_bounds if edx is less than 0  */

	cmpl %r10d, %esi 	/* Compare r10d with esi */
	jge .Lout_of_bounds	/* jump to Lout_of_bounds if esi is greater than or equal to r10 */
	
	cmpl %r11d, %edx 	/* Compare r11d with edx */
	jge .Lout_of_bounds /* jump to Lout_of_bounds if edx is greater than or equal to r11 */

	movl $1, %eax 		/* move 1 to eax return registrar */
	jmp .Lin_bounds_end /* jump to end */

	.Lout_of_bounds:
	movl $0, %eax 		/* move 0 to eax return registrar */
	
	.Lin_bounds_end:
	ret

/*
 * Computes the index of a pixel in an image’s data array given its x and y coordinates
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate
 *   %edx     - y coordinate
 */
	.globl compute_index
compute_index:
	movl IMAGE_WIDTH_OFFSET(%rdi), %eax		/* offset Image to get width and store in return value register */
	imull %edx, %eax						/* multiply value by y coordinate */
	addl %esi, %eax							/* add x coordinate to return value */
	ret

/* Constrains a value so that it is greater than or equal to min and less than or equal to max.
 *
 * Parameters:
 *   %edi     - pointer to struct Image
 *   %esi     - minimum value
 *   %edx     - maximum value
 */
	.globl clamp
clamp:
	movl %edi, %eax		/* move 1st argument register in func call to return value register */

	cmpl %edx, %edi		/* Compare edx and edi */
	jg .Lclamp_max		/* jump to Lmax If edi is greater than edx */

  	cmpl %esi, %edi 	/* Compare esi and dsi */
	jl .Lclamp_min 		/* jump to Lmin If edi is less than esi */

	jmp .Lclamp_end 	/* Jump to the end */

.Lclamp_max:
	movl %edx, %eax 	/* move 3rd argument register in func call to return value register  */
	jmp .Lclamp_end 	/* Jump to the end */

.Lclamp_min:
	movl %esi, %eax 	/* move 2nd argument register in func call to return value register */

.Lclamp_end:
	ret

/*
 * Returns the red component of a pixel color value
 *
 * Parameters:
 *   %edi     - uint32_t color value
 */
	.globl get_r
get_r:
	movl %edi, %eax		/* move 1st argument register in func call to return value register */
	shrl $24, %eax		/* shift to the left 24 bits */
	andl $255, %eax		/* bitwise and the return value with 255 */
	ret

/*
 * Returns the green component of a pixel color value
 *
 * Parameters:
 *   %edi     - uint32_t color value
 */
	.globl get_g
get_g:
	movl %edi, %eax		/* move 1st argument register in func call to return value register */
	shrl $16, %eax		/* shift to the left 16 bits */
	andl $255, %eax		/* bitwise and the return value with 255 */
	ret

/*
 * Returns the blue component of a pixel color value
 *
 * Parameters:
 *   %edi     - uint32_t color value
 */
	.globl get_b
get_b:
	movl %edi, %eax		/* move 1st argument register in func call to return value register */
	shrl $8, %eax		/* shift to the left 8 bits */
	andl $255, %eax		/* bitwise and the return value with 255 */
	ret

/*
 * Returns the alpha component of a pixel color value
 *
 * Parameters:
 *   %edi     - uint32_t color value
 */
	.globl get_a
get_a:
	movl %edi, %eax		/* move 1st argument register in func call to return value register */
	andl $255, %eax		/* bitwise and the return value with 255 */
	ret

/* Blends foreground and background color component values using a specified alpha (opacity) value.
 *
 * Parameters:
 *   %edi     - foreground color of pixel
 *   %esi     - background color of pixel
 *   %edx     - alpha value of pixel
 */
	.globl blend_components
blend_components:
	movl $255, %eax 	/* move $255 to return value registrar */
	subl %edx, %eax 	/* subtract %edx from return value registrar */
	imull %esi, %eax 	/* multiply %esi with return value registrar */

	movl %edx, %r10d 	/* move %edx to %r10d temp registrar */
	imull %edi, %r10d 	/* multiply %edi with return registrar */

	addl %r10d, %eax 	/* add %r10d temp registrar to %eax */
	movl $255, %r10d 	/* move 255 to %r10d registrar */
	movl $0, %edx 		/* Set edx register to 0 to get ready divide */
 
	div %r10d 			/* divide %eax registrar by %r10d registrar */
	ret
	
/* Constrains a value so that it is greater than or equal to min and less than or equal to max.
 *
 * Parameters:
 *   %edi     - pointer to struct Image
 *   %esi     - index value of the the pixel in the image's data array
 *   %edx     - value of the color to blend into
 */
	.globl set_pixel
set_pixel:
	pushq %r12		/* push callee save register to store pixel address */
	pushq %r13		/* push callee save register to store index value */
	subq $8, %rsp	/* subtract 8 so that rsp is at an address that's a multiple of 32 */

	movq IMAGE_DATA_OFFSET(%rdi), %r12 	/* move pointer to the start of the data array and store in r12*/
	movl %esi, %r13d 						/* store the index value in r13 */

	movl %edx, %edi 					/* store color in the first argument */
	movl (%r12, %r13, 4), %esi 			/* store the pointer to the pixel (start of data + 4*index) in the second argument */
	call blend_colors					/* call blend_colors with the color and pointer */
	movl %eax, (%r12, %r13, 4)			/* move the result of blend_colors to the pixel (start of data + 4*index) */

	addq $8, %rsp 	/* Decrement rsp register by 8 to restore the value */
	popq %r12		/* restore r12 */		
	popq %r13		/* restore r13 */
	ret

/** Blends foreground and background colors using the foreground color’s alpha value to produce an opaque color
*
* Parameters:
*   %edi     - value of the foreground pixel
*   %esi     - value of the background pixel
*/
	.globl blend_colors
blend_colors:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$8, %rsp
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 3, -40
	movl	%edi, -28(%rbp)
	movl	%esi, -32(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	get_a
	movzbl	%al, %r12d
	movl	-32(%rbp), %eax
	movl	%eax, %edi
	call	get_r
	movzbl	%al, %ebx
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	get_r
	movzbl	%al, %eax
	movl	%r12d, %edx
	movl	%ebx, %esi
	movl	%eax, %edi
	call	blend_components
	movzbl	%al, %eax
	sall	$24, %eax
	movl	%eax, %r13d
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	get_a
	movzbl	%al, %r12d
	movl	-32(%rbp), %eax
	movl	%eax, %edi
	call	get_g
	movzbl	%al, %ebx
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	get_g
	movzbl	%al, %eax
	movl	%r12d, %edx
	movl	%ebx, %esi
	movl	%eax, %edi
	call	blend_components
	movzbl	%al, %eax
	sall	$16, %eax
	addl	%eax, %r13d
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	get_a
	movzbl	%al, %r12d
	movl	-32(%rbp), %eax
	movl	%eax, %edi
	call	get_b
	movzbl	%al, %ebx
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	get_b
	movzbl	%al, %eax
	movl	%r12d, %edx
	movl	%ebx, %esi
	movl	%eax, %edi
	call	blend_components
	movzbl	%al, %eax
	sall	$8, %eax
	addl	%r13d, %eax
	orb	$-1, %al
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
	
/* Returns the result of squaring an int64_t value.
 *
 * Parameters:
 *   %rdi     - the value to be squared
 */
	.globl square
square:
	movq %rdi, %rax		/* move 1st argument register in func call to return value register */
	imulq %rax, %rax	/* multiply by itself */
	ret

/* Returns the sum of the squares of the x and y distance between two points
 * 		return square(x1 - x2) + square(y1 - y2);
 * Parameters:
 *   %rdi     - the x value of the first point (x1)
 *   %rsi     - the y value of the first point (y1)
 *   %rdx     - the x value of the second point (x2)
 *   %rcx     - the y value of the second point (y2)
 */
	.globl square_dist
square_dist:
	pushq %r12			/* push r12 onto the stack to push the x2-x1 vaule */
	subq $8, %rsp		/* subtract 8 so that rsp is at an address that's a multiple of 32 */

	subq %rdx, %rdi		/* subtract x2 from x1 */
	call square			/* call square function with rdi = x2 - x1*/
	movq %rax, %r12		/* store result in r12 */

	subq %rcx, %rsi		/* subtract y2 from y1 */
	movq %rsi, %rdi		/* move y2-y1 to rdi */
	call square			/* call square function with rdi = y2 - y1 */
	addq %rax, %r12		/* add the result to the temporary register  */

	movq %r12, %rax		/* move the result to the return value register */
	
	addq $8, %rsp 		/* Decrement rsp register by 8 to restore the value */
	popq %r12; 			/* restore r12 */
	
	ret

/***********************************************************************
   Public API functions
 ***********************************************************************/

/*
 * Draw a pixel.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate (pixel column)
 *   %edx     - y coordinate (pixel row)
 *   %ecx     - uint32_t color value
 */
	.globl draw_pixel
draw_pixel:
	pushq %rbx /* push %rbx callee saved register onto stack */
	pushq %r12 /* push %r12 callee saved register onto stack*/
	pushq %r13 /* push %r13 callee saved register onto stack*/

	movq %rdi, %rbx /* move the rdi value containing the pointer to struct image to rbx register */
	movl %esi, %r12d /* move the esi value containing the x coordinate to r12d register */
	movl %edx, %r13d /* move the edx value containing the x coordinate to r13d register */

	call in_bounds /* call in_bounds*/
	cmpl $1, %eax /* Compare to see if eax is less than 1 to see if its out of bounds*/
	jg .Ldraw_pixel_end /* If the pixel is out of bounds, jump to the end */

	movq %rbx, %rdi /* move the rbx value containing the pointer to struct image back to rdi register*/
	movl %r12d, %esi /* move the r12d value containing the x coordinate back to esi register*/
	movl %r13d, %edx /* move the r13d value containing the y coordinate back to edx register*/
	call compute_index /* call compute index */

	movl %eax, %esi /*  move the result of compute index to the esi register to prep for set_pixel call */
	movl %ecx, %edx /*  move the color value stored at ecx to the edx register to prep for set_pixel call */
	movq %rbx, %rdi /* move the rbx value containing the pointer to struct image back to rdi register*/
	call set_pixel /* call set_pixel */

.Ldraw_pixel_end:
	popq %r13 /* restore r13 saved register */
	popq %r12 /* restore r12 saved register */
	popq %rbx /* restore rbx saved register */

	ret


/*
 * Draw a rectangle.
 * The rectangle has rect->x,rect->y as its upper left corner,
 * is rect->width pixels wide, and rect->height pixels high.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %rsi     - pointer to struct Rect
 *   %edx     - uint32_t color value
 */
	.globl draw_rect
draw_rect:
	/* TODO: implement */
	ret

/*
 * Draw a circle.
 * The circle has x,y as its center and has r as its radius.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate of circle's center
 *   %edx     - y coordinate of circle's center
 *   %ecx     - radius of circle
 *   %r8d     - uint32_t color value
 */
	.globl draw_circle
draw_circle:
	/* TODO: implement */
	ret

/*
 * Draw a tile by copying all pixels in the region
 * enclosed by the tile parameter in the tilemap image
 * to the specified x/y coordinates of the destination image.
 * No blending of the tile pixel colors with the background
 * colors should be done.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 *   %rcx - pointer to Image (the tilemap)
 *   %r8  - pointer to Rect (the tile)
 */
	.globl draw_tile
draw_tile:
	/* TODO: implement */
	ret

/*
 * Draw a sprite by copying all pixels in the region
 * enclosed by the sprite parameter in the spritemap image
 * to the specified x/y coordinates of the destination image.
 * The alpha values of the sprite pixels should be used to
 * blend the sprite pixel colors with the background
 * pixel colors.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where sprite should be copied
 *   %edx - y coordinate of location where sprite should be copied
 *   %rcx - pointer to Image (the spritemap)
 *   %r8  - pointer to Rect (the sprite)
 */
	.globl draw_sprite
draw_sprite:
	/* TODO: implement */
	ret

/*
vim:ft=gas:
*/
