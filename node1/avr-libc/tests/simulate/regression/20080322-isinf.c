/* Copyright (c) 2008  Dmitry Xmelkov
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

/* Bug: isinf() produces too large code with GCC 4.3.0
   $Id$

   GCC 4.3.0 unrolls the isinff() function with call of __unordsf2().
   The testing method is to abort on big code size.
   As a work around, -fno-builtin-isinff can be used.

   Notice that this is rather a GCC issue PR115307, fixed in v14.2+
   and v13.4.  And similar problems persist for isnan().  */

#ifdef	__AVR__

extern int isinff (float);
extern int theSize[];

int use_isinff (float x)
{
    int res;
    __asm volatile (".Lsize = ."
                    : "+r" (x) :: "memory");

    res = isinff (x);

    __asm volatile (".global theSize\n\t"
                    "theSize = . - .Lsize"
                    : "+r" (res) :: "memory");
    return res;
}


int main (void)
{
    if ((unsigned) theSize > 20)
        __builtin_exit (__LINE__);

    return 0;
}

#else

int main ()
{
    return 0;
}
#endif
