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
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r11d	/* move pointer value of image height to temporary registrar %r11 */
	
	cmpl $0, %esi		/* Compare 0 with x coordinate */
	jl .Lout_of_bounds	/* jump to Lout_of_bounds if x coordinate is less than 0  */

	cmpl $0, %edx		/* Compare 0 with y coordinate */
	jl .Lout_of_bounds 	/* jump to Lout_of_bounds if y coordinate is less than 0  */

	cmpl %r10d, %esi 	/* Compare pointer value with x coordinate */
	jge .Lout_of_bounds	/* jump to Lout_of_bounds if y is greater than or equal to pointer value of image width */
	
	cmpl %r11d, %edx 	/* Compare pointer value of image height with y coordinate */
	jge .Lout_of_bounds /* jump to Lout_of_bounds if y is greater than or equal to pointer value of image height */

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
	movl %edi, %eax		/* move pointer to struct image in func call to return value register */

	cmpl %edx, %edi		/* Compare maximum value and value at pointer to struct image */
	jg .Lclamp_max		/* jump to Lmax if value at pointer is greater */

  	cmpl %esi, %edi 	/* Compare esi and dsi */
	jl .Lclamp_min 		/* jump to Lmin If value at pointer is smaller */

	jmp .Lclamp_end 	/* Jump to the end */

.Lclamp_max:
	movl %edx, %eax 	/* move max value to return value register  */
	jmp .Lclamp_end 	/* Jump to the end */

.Lclamp_min:
	movl %esi, %eax 	/* move min value to return value register */

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
	movl %edi, %eax		/* move color value in func call to return value register */
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
	movl %edi, %eax		/* move color value in func call to return value register */
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
	movl %edi, %eax		/* move color value in func call to return value register */
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
	movl %edi, %eax		/* move color value in func call to return value register */
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
	subl %edx, %eax 	/* subtract the alpha value of the pixel from $255 */
	imull %esi, %eax 	/* multiply foreground color value with return value registrar */

	movl %edx, %r10d 	/* move alpha value of pixel to %r10d temp registrar */
	imull %edi, %r10d 	/* multiply value of forground color of pixel with temp registrar */

	addl %r10d, %eax 	/* add %r10d temp registrar to return registrar*/
	movl $255, %r10d 	/* move 255 to %r10d temp registrar */
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
	pushq %rbx /* push %rbx callee saved register onto stack */
 	pushq %r12 /* push %r12 register onto stack */
	pushq %r13 /* push %r13 register onto stack */
	pushq %r14 /* push %r14 register onto stack */
	pushq %r15 /* push %r15 register onto stack */

	movl %edi, %r14d /* move the value of foreground pixel to saved registrar %r14d */
	movl %esi, %r15d /* move the value of background pixel to saved registrar %r15d */

	call get_a /* call get_a function */
	movl %eax, %r12d /* move the returned value from get_a to saved registrar %r12d */

	movl %r14d, %edi /* move the value of foreground pixel to %edi for get_r*/
	call get_r /* call get_r function */
	movl %eax, %r13d /* move the returned value from get_r to saved registrar %r13d */

	movl %r15d, %edi /* move the value of the background pixel to %edi to set up get_r call */
	call get_r /* call get_r function */
	movl %eax, %esi /* move the returned value from get_r to saved registrar %esi */

	movl %r13d, %edi /* move the value of get_r register to %edi to prep for blend_components call*/
	movl %r12d, %edx /* move the value of get_a call pixel to %edi to prep for blend_components call*/
	call blend_components /* call blend_components */
	shll $24, %eax  /* Left shift ebx register 24 times */
	movl %eax, %ebx /* Move the return result to the %ebx saved register */


	movl %r14d, %edi /* move the value of foreground pixel to %edi for get_g*/
	call get_g /* call get_g function */
	movl %eax, %r13d /* move the returned value from get_g to saved registrar %r13d */

	movl %edi, %r14d /* move the value of foreground pixel to saved registrar %r14d */
	movl %r15d, %edi /* move the value of the background pixel to %edi to set up get_g call */
	call get_g /* call get_g function */
	movl %eax, %esi /* move the returned value from get_g to saved registrar %esi */

	movl %r13d, %edi /* move the value of foreground pixel register to %edi to prep for blend_components call*/
	movl %r12d, %edx /* move the value of get_a call pixel to %edi to prep for blend_components call*/
	call blend_components /* call blend_components */
	shll $16, %eax  /* Left shift ebx register 16 times */
	addl %eax, %ebx /* Add the return result to the %ebx saved register */


	movl %r14d, %edi /* move the value of foreground pixel to %edi for get_b*/
	call get_b /* call get_b function */
	movl %eax, %r13d /* move the returned value from get_b to saved registrar %r13d */

	movl %edi, %r14d /* move the value of foreground pixel to saved registrar %r14d */
	movl %r15d, %edi /* move the value of the background pixel to %edi to set up get_b call */
	call get_b /* call get_b function */
	movl %eax, %esi /* move the returned value from get_g to saved registrar %esi */

	movl %r13d, %edi /* move the value of foreground pixel register to %edi to prep for blend_components call*/
	movl %r12d, %edx /* move the value of get_a call pixel to %edi to prep for blend_components call*/
	call blend_components /* call blend_components */
	shll $8, %eax  /* Left shift ebx register 8 times */
	addl %ebx, %eax /* Add the %ebx register to the the return result */
	orl  $255, %eax	/* or the result with 255 */

	popq %r15 /* Restore r15  saved register */
	popq %r14 /* Restore r14  saved register */
	popq %r13 /* Restore r13  saved register */
	popq %r12 /* Restore r12  saved register */
	popq %rbx /* Restore rbx  saved register */
	ret
	
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
	jl .Ldraw_pixel_end /* If the pixel is out of bounds, jump to the end */

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
   /* Callee stored pointers: 
	* %rbp	- allocated buffer pointer
	* %rbx	- pointer to struct Image
	* %r12	- pointer to struct Rect
	* %r13d	- uint32_t color value
	* %r14d	- x int32_t iterator value
	* %r15d	- y int32_t iterator value
	*
	* Stack Alocated
	* 0(%rbp)	- stores max x value (rect->x + rect->width)
	* 4(%rbp)	- stores max y value (rect->y + rect->height)
	*/


	/* Saving callee values */
	pushq %rbp /* store what ever address rbp is */
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	subq $8, %rsp /* allocating 8 bytes */
	movq %rsp, %rbp /* point %rbp to alloc'ed buf */

	/* store arguments */
	movq %rdi, %rbx /* store Image pointer */
	movq %rsi, %r12 /* store Rect pointer */
	movl %edx, %r13d /* store color value */
	
	/* iterator value */ 
	movl RECT_X_OFFSET(%r12), %r14d /* x should initialize to rect->x */

	/* calculate limits */
	/* rect->x + rect->width */
	movl RECT_X_OFFSET(%r12), %r10d /* store rect->x on stack */
	addl RECT_WIDTH_OFFSET(%r12), %r10d /* add rect->width to the value on stack */
	movl %r10d, 0(%rbp) /* add rect->width to the value on stack */
	/* rect->y + rect->height */
	movl RECT_Y_OFFSET(%r12), %r10d /* store rect->y on stack */
	addl RECT_HEIGHT_OFFSET(%r12), %r10d /* add rect->height to the value on stack */	
	movl %r10d, 4(%rbp) /* add rect->height to the value on stack */	

/* top of outter loop */
.Lrect_x_top:
	movl RECT_Y_OFFSET(%r12), %r15d /* y should initialize to rect->y*/
	cmp 0(%rbp), %r14d /* if x is >= max x value, jump to very end */
	jge .Lrect_x_end

.Lrect_y_top:
	cmp 4(%rbp), %r15d /* if y is >= max y value, jump to very end */
	jge .Lrect_y_end

	/* call draw pixel after moving arguments */
	movq %rbx, %rdi
	movl %r14d, %esi
	movl %r15d, %edx
	movl %r13d, %ecx 
	call draw_pixel

	inc %r15d
	jmp .Lrect_y_top


.Lrect_y_end:
	inc %r14d
	jmp .Lrect_x_top


.Lrect_x_end:

	addq $8, %rsp /* restore stack pointer */

	/* Restore callee values */
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
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
