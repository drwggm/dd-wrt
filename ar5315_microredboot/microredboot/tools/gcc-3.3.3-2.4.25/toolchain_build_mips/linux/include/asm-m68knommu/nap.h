/****************************************************************************/

/*
 *	nap.h -- Gilbarco/NAP support.
 *
 * 	(C) Copyright 2001-2003, SnapGear (www.snapgear.com) 
 */

/****************************************************************************/
#ifndef	nap_h
#define	nap_h
/****************************************************************************/

#include <linux/config.h>

/****************************************************************************/
#ifdef CONFIG_GILBARCONAP
/****************************************************************************/

#ifdef CONFIG_COLDFIRE
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#endif

/*
 *	Command to support selecting RS232 or RS422 mode on the
 *	second COM port.
 */
#define	TIOCSET422	0x4d01		/* Set port mode 232 or 422 */
#define	TIOCGET422	0x4d02		/* Get current port mode */

/*
 *	Low level control of the RS232/422 enable.
 */
#define	MCFPP_PA11	0x0800

#ifndef __ASSEMBLY__
/*
 *	RS232/422 control is via the single PA11 line. Low is the RS422
 *	enable, high is RS232 mode.
 */
static __inline__ unsigned int mcf_getpa(void)
{
	volatile unsigned short *pp;
	pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT);
	return((unsigned int) *pp);
}

static __inline__ void mcf_setpa(unsigned int mask, unsigned int bits)
{
	volatile unsigned short *pp;
	unsigned long		flags;

	pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT);
	save_flags(flags); cli();
	*pp = (*pp & ~mask) | bits;
	restore_flags(flags);
}
#endif /* __ASSEMBLY__ */

/****************************************************************************/

#if defined(CONFIG_M5272)
/*
 *	Gilbarco/NAP based hardware. DTR/DCD lines are wired to GPB lines.
 */
#define	MCF_HAVEDCD0
#define	MCF_HAVEDCD1
#define	MCF_HAVEDTR0
#define	MCF_HAVEDTR1

#ifndef __ASSEMBLY__
/*
 *	Functions to abstract access to the DCD/DTR lines. There is no
 *	standard way to implement these on ColdFire, mostly spare PPIO
 *	bits are used.
 */
static __inline__ unsigned int mcf_getppdcd(unsigned int portnr)
{
	volatile unsigned short *pp;
#ifdef CONFIG_SMC91111
	if (portnr == 0) {
		pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PBDAT);
		return((*pp & 0x0080) ? 0 : 1);
	} else if (portnr == 1) {
		pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT);
		return((*pp & 0x0004) ? 0 : 1);
	}
#else
	if (portnr < 2) {
		pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PBDAT);
		return((*pp & (portnr ? 0x0020 : 0x0080)) ? 0 : 1);
	}
#endif
	return(0);
}
static __inline__ unsigned int mcf_getppdtr(unsigned int portnr)
{
	volatile unsigned short *pp;
	if (portnr < 2) {
		pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PBDAT);
		return((*pp & (portnr ? 0x0010 : 0x0040)) ? 0 : 1);
	}
	return(0);
}

static __inline__ void mcf_setppdtr(unsigned int portnr, unsigned int dtr)
{
	volatile unsigned short *pp;
	unsigned short bit;
	if (portnr < 2) {
		pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PBDAT);
		bit = (portnr ? 0x0010 : 0x0040);
		*pp = (*pp & ~bit) | (dtr ? 0 : bit);
	}
}
#endif /* __ASSEMBLY__ */
#endif /* CONFIG_M5272 */

/****************************************************************************/
#endif /* CONFIG_GILBARCONAP */
/****************************************************************************/
#endif	/* nap_h */
