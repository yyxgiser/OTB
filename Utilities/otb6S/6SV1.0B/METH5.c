/* METH5.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    meth5(a,inu) >*/
/* Subroutine */ int meth5_(doublereal *a, integer *inu)
{
    /* Initialized data */

    static doublereal acr[2048]	/* was [8][256] */ = { 0.,0.,0.,0.,0.,0.,
	    12740.,12750.,0.,0.,0.,0.,0.,0.,12750.,12760.,0.,0.,0.,0.,0.,0.,
	    12760.,12770.,0.,0.,0.,0.,0.,0.,12770.,12780.,0.,0.,0.,0.,0.,0.,
	    12780.,12790.,0.,0.,0.,0.,0.,0.,12790.,12800.,0.,0.,0.,0.,0.,0.,
	    12800.,12810.,0.,0.,0.,0.,0.,0.,12810.,12820.,0.,0.,0.,0.,0.,0.,
	    12820.,12830.,0.,0.,0.,0.,0.,0.,12830.,12840.,0.,0.,0.,0.,0.,0.,
	    12840.,12850.,0.,0.,0.,0.,0.,0.,12850.,12860.,0.,0.,0.,0.,0.,0.,
	    12860.,12870.,0.,0.,0.,0.,0.,0.,12870.,12880.,0.,0.,0.,0.,0.,0.,
	    12880.,12890.,0.,0.,0.,0.,0.,0.,12890.,12900.,0.,0.,0.,0.,0.,0.,
	    12900.,12910.,0.,0.,0.,0.,0.,0.,12910.,12920.,0.,0.,0.,0.,0.,0.,
	    12920.,12930.,0.,0.,0.,0.,0.,0.,12930.,12940.,0.,0.,0.,0.,0.,0.,
	    12940.,12950.,0.,0.,0.,0.,0.,0.,12950.,12960.,0.,0.,0.,0.,0.,0.,
	    12960.,12970.,0.,0.,0.,0.,0.,0.,12970.,12980.,0.,0.,0.,0.,0.,0.,
	    12980.,12990.,0.,0.,0.,0.,0.,0.,12990.,1.3e4,0.,0.,0.,0.,0.,0.,
	    1.3e4,13010.,0.,0.,0.,0.,0.,0.,13010.,13020.,0.,0.,0.,0.,0.,0.,
	    13020.,13030.,0.,0.,0.,0.,0.,0.,13030.,13040.,0.,0.,0.,0.,0.,0.,
	    13040.,13050.,0.,0.,0.,0.,0.,0.,13050.,13060.,0.,0.,0.,0.,0.,0.,
	    13060.,13070.,0.,0.,0.,0.,0.,0.,13070.,13080.,0.,0.,0.,0.,0.,0.,
	    13080.,13090.,0.,0.,0.,0.,0.,0.,13090.,13100.,0.,0.,0.,0.,0.,0.,
	    13100.,13110.,0.,0.,0.,0.,0.,0.,13110.,13120.,0.,0.,0.,0.,0.,0.,
	    13120.,13130.,0.,0.,0.,0.,0.,0.,13130.,13140.,0.,0.,0.,0.,0.,0.,
	    13140.,13150.,0.,0.,0.,0.,0.,0.,13150.,13160.,0.,0.,0.,0.,0.,0.,
	    13160.,13170.,0.,0.,0.,0.,0.,0.,13170.,13180.,0.,0.,0.,0.,0.,0.,
	    13180.,13190.,0.,0.,0.,0.,0.,0.,13190.,13200.,0.,0.,0.,0.,0.,0.,
	    13200.,13210.,0.,0.,0.,0.,0.,0.,13210.,13220.,0.,0.,0.,0.,0.,0.,
	    13220.,13230.,0.,0.,0.,0.,0.,0.,13230.,13240.,0.,0.,0.,0.,0.,0.,
	    13240.,13250.,0.,0.,0.,0.,0.,0.,13250.,13260.,0.,0.,0.,0.,0.,0.,
	    13260.,13270.,0.,0.,0.,0.,0.,0.,13270.,13280.,0.,0.,0.,0.,0.,0.,
	    13280.,13290.,0.,0.,0.,0.,0.,0.,13290.,13300.,0.,0.,0.,0.,0.,0.,
	    13300.,13310.,0.,0.,0.,0.,0.,0.,13310.,13320.,0.,0.,0.,0.,0.,0.,
	    13320.,13330.,0.,0.,0.,0.,0.,0.,13330.,13340.,0.,0.,0.,0.,0.,0.,
	    13340.,13350.,0.,0.,0.,0.,0.,0.,13350.,13360.,0.,0.,0.,0.,0.,0.,
	    13360.,13370.,0.,0.,0.,0.,0.,0.,13370.,13380.,0.,0.,0.,0.,0.,0.,
	    13380.,13390.,0.,0.,0.,0.,0.,0.,13390.,13400.,0.,0.,0.,0.,0.,0.,
	    13400.,13410.,0.,0.,0.,0.,0.,0.,13410.,13420.,0.,0.,0.,0.,0.,0.,
	    13420.,13430.,0.,0.,0.,0.,0.,0.,13430.,13440.,0.,0.,0.,0.,0.,0.,
	    13440.,13450.,0.,0.,0.,0.,0.,0.,13450.,13460.,0.,0.,0.,0.,0.,0.,
	    13460.,13470.,0.,0.,0.,0.,0.,0.,13470.,13480.,0.,0.,0.,0.,0.,0.,
	    13480.,13490.,0.,0.,0.,0.,0.,0.,13490.,13500.,0.,0.,0.,0.,0.,0.,
	    13500.,13510.,0.,0.,0.,0.,0.,0.,13510.,13520.,0.,0.,0.,0.,0.,0.,
	    13520.,13530.,0.,0.,0.,0.,0.,0.,13530.,13540.,0.,0.,0.,0.,0.,0.,
	    13540.,13550.,0.,0.,0.,0.,0.,0.,13550.,13560.,0.,0.,0.,0.,0.,0.,
	    13560.,13570.,0.,0.,0.,0.,0.,0.,13570.,13580.,0.,0.,0.,0.,0.,0.,
	    13580.,13590.,0.,0.,0.,0.,0.,0.,13590.,13600.,0.,0.,0.,0.,0.,0.,
	    13600.,13610.,0.,0.,0.,0.,0.,0.,13610.,13620.,0.,0.,0.,0.,0.,0.,
	    13620.,13630.,0.,0.,0.,0.,0.,0.,13630.,13640.,0.,0.,0.,0.,0.,0.,
	    13640.,13650.,0.,0.,0.,0.,0.,0.,13650.,13660.,0.,0.,0.,0.,0.,0.,
	    13660.,13670.,0.,0.,0.,0.,0.,0.,13670.,13680.,0.,0.,0.,0.,0.,0.,
	    13680.,13690.,0.,0.,0.,0.,0.,0.,13690.,13700.,0.,0.,0.,0.,0.,0.,
	    13700.,13710.,0.,0.,0.,0.,0.,0.,13710.,13720.,0.,0.,0.,0.,0.,0.,
	    13720.,13730.,0.,0.,0.,0.,0.,0.,13730.,13740.,0.,0.,0.,0.,0.,0.,
	    13740.,13750.,0.,0.,0.,0.,0.,0.,13750.,13760.,0.,0.,0.,0.,0.,0.,
	    13760.,13770.,0.,0.,0.,0.,0.,0.,13770.,13780.,0.,0.,0.,0.,0.,0.,
	    13780.,13790.,0.,0.,0.,0.,0.,0.,13790.,13800.,0.,0.,0.,0.,0.,0.,
	    13800.,13810.,0.,0.,0.,0.,0.,0.,13810.,13820.,0.,0.,0.,0.,0.,0.,
	    13820.,13830.,0.,0.,0.,0.,0.,0.,13830.,13840.,0.,0.,0.,0.,0.,0.,
	    13840.,13850.,0.,0.,0.,0.,0.,0.,13850.,13860.,0.,0.,0.,0.,0.,0.,
	    13860.,13870.,0.,0.,0.,0.,0.,0.,13870.,13880.,0.,0.,0.,0.,0.,0.,
	    13880.,13890.,0.,0.,0.,0.,0.,0.,13890.,13900.,0.,0.,0.,0.,0.,0.,
	    13900.,13910.,0.,0.,0.,0.,0.,0.,13910.,13920.,0.,0.,0.,0.,0.,0.,
	    13920.,13930.,0.,0.,0.,0.,0.,0.,13930.,13940.,0.,0.,0.,0.,0.,0.,
	    13940.,13950.,0.,0.,0.,0.,0.,0.,13950.,13960.,0.,0.,0.,0.,0.,0.,
	    13960.,13970.,0.,0.,0.,0.,0.,0.,13970.,13980.,0.,0.,0.,0.,0.,0.,
	    13980.,13990.,0.,0.,0.,0.,0.,0.,13990.,1.4e4,0.,0.,0.,0.,0.,0.,
	    1.4e4,14010.,0.,0.,0.,0.,0.,0.,14010.,14020.,0.,0.,0.,0.,0.,0.,
	    14020.,14030.,0.,0.,0.,0.,0.,0.,14030.,14040.,0.,0.,0.,0.,0.,0.,
	    14040.,14050.,0.,0.,0.,0.,0.,0.,14050.,14060.,0.,0.,0.,0.,0.,0.,
	    14060.,14070.,0.,0.,0.,0.,0.,0.,14070.,14080.,0.,0.,0.,0.,0.,0.,
	    14080.,14090.,0.,0.,0.,0.,0.,0.,14090.,14100.,0.,0.,0.,0.,0.,0.,
	    14100.,14110.,0.,0.,0.,0.,0.,0.,14110.,14120.,0.,0.,0.,0.,0.,0.,
	    14120.,14130.,0.,0.,0.,0.,0.,0.,14130.,14140.,0.,0.,0.,0.,0.,0.,
	    14140.,14150.,0.,0.,0.,0.,0.,0.,14150.,14160.,0.,0.,0.,0.,0.,0.,
	    14160.,14170.,0.,0.,0.,0.,0.,0.,14170.,14180.,0.,0.,0.,0.,0.,0.,
	    14180.,14190.,0.,0.,0.,0.,0.,0.,14190.,14200.,0.,0.,0.,0.,0.,0.,
	    14200.,14210.,0.,0.,0.,0.,0.,0.,14210.,14220.,0.,0.,0.,0.,0.,0.,
	    14220.,14230.,0.,0.,0.,0.,0.,0.,14230.,14240.,0.,0.,0.,0.,0.,0.,
	    14240.,14250.,0.,0.,0.,0.,0.,0.,14250.,14260.,0.,0.,0.,0.,0.,0.,
	    14260.,14270.,0.,0.,0.,0.,0.,0.,14270.,14280.,0.,0.,0.,0.,0.,0.,
	    14280.,14290.,0.,0.,0.,0.,0.,0.,14290.,14300.,0.,0.,0.,0.,0.,0.,
	    14300.,14310.,0.,0.,0.,0.,0.,0.,14310.,14320.,0.,0.,0.,0.,0.,0.,
	    14320.,14330.,0.,0.,0.,0.,0.,0.,14330.,14340.,0.,0.,0.,0.,0.,0.,
	    14340.,14350.,0.,0.,0.,0.,0.,0.,14350.,14360.,0.,0.,0.,0.,0.,0.,
	    14360.,14370.,0.,0.,0.,0.,0.,0.,14370.,14380.,0.,0.,0.,0.,0.,0.,
	    14380.,14390.,0.,0.,0.,0.,0.,0.,14390.,14400.,0.,0.,0.,0.,0.,0.,
	    14400.,14410.,0.,0.,0.,0.,0.,0.,14410.,14420.,0.,0.,0.,0.,0.,0.,
	    14420.,14430.,0.,0.,0.,0.,0.,0.,14430.,14440.,0.,0.,0.,0.,0.,0.,
	    14440.,14450.,0.,0.,0.,0.,0.,0.,14450.,14460.,0.,0.,0.,0.,0.,0.,
	    14460.,14470.,0.,0.,0.,0.,0.,0.,14470.,14480.,0.,0.,0.,0.,0.,0.,
	    14480.,14490.,0.,0.,0.,0.,0.,0.,14490.,14500.,0.,0.,0.,0.,0.,0.,
	    14500.,14510.,0.,0.,0.,0.,0.,0.,14510.,14520.,0.,0.,0.,0.,0.,0.,
	    14520.,14530.,0.,0.,0.,0.,0.,0.,14530.,14540.,0.,0.,0.,0.,0.,0.,
	    14540.,14550.,0.,0.,0.,0.,0.,0.,14550.,14560.,0.,0.,0.,0.,0.,0.,
	    14560.,14570.,0.,0.,0.,0.,0.,0.,14570.,14580.,0.,0.,0.,0.,0.,0.,
	    14580.,14590.,0.,0.,0.,0.,0.,0.,14590.,14600.,0.,0.,0.,0.,0.,0.,
	    14600.,14610.,0.,0.,0.,0.,0.,0.,14610.,14620.,0.,0.,0.,0.,0.,0.,
	    14620.,14630.,0.,0.,0.,0.,0.,0.,14630.,14640.,0.,0.,0.,0.,0.,0.,
	    14640.,14650.,0.,0.,0.,0.,0.,0.,14650.,14660.,0.,0.,0.,0.,0.,0.,
	    14660.,14670.,0.,0.,0.,0.,0.,0.,14670.,14680.,0.,0.,0.,0.,0.,0.,
	    14680.,14690.,0.,0.,0.,0.,0.,0.,14690.,14700.,0.,0.,0.,0.,0.,0.,
	    14700.,14710.,0.,0.,0.,0.,0.,0.,14710.,14720.,0.,0.,0.,0.,0.,0.,
	    14720.,14730.,0.,0.,0.,0.,0.,0.,14730.,14740.,0.,0.,0.,0.,0.,0.,
	    14740.,14750.,0.,0.,0.,0.,0.,0.,14750.,14760.,0.,0.,0.,0.,0.,0.,
	    14760.,14770.,0.,0.,0.,0.,0.,0.,14770.,14780.,0.,0.,0.,0.,0.,0.,
	    14780.,14790.,0.,0.,0.,0.,0.,0.,14790.,14800.,0.,0.,0.,0.,0.,0.,
	    14800.,14810.,0.,0.,0.,0.,0.,0.,14810.,14820.,0.,0.,0.,0.,0.,0.,
	    14820.,14830.,0.,0.,0.,0.,0.,0.,14830.,14840.,0.,0.,0.,0.,0.,0.,
	    14840.,14850.,0.,0.,0.,0.,0.,0.,14850.,14860.,0.,0.,0.,0.,0.,0.,
	    14860.,14870.,0.,0.,0.,0.,0.,0.,14870.,14880.,0.,0.,0.,0.,0.,0.,
	    14880.,14890.,0.,0.,0.,0.,0.,0.,14890.,14900.,0.,0.,0.,0.,0.,0.,
	    14900.,14910.,0.,0.,0.,0.,0.,0.,14910.,14920.,0.,0.,0.,0.,0.,0.,
	    14920.,14930.,0.,0.,0.,0.,0.,0.,14930.,14940.,0.,0.,0.,0.,0.,0.,
	    14940.,14950.,0.,0.,0.,0.,0.,0.,14950.,14960.,0.,0.,0.,0.,0.,0.,
	    14960.,14970.,0.,0.,0.,0.,0.,0.,14970.,14980.,0.,0.,0.,0.,0.,0.,
	    14980.,14990.,0.,0.,0.,0.,0.,0.,14990.,1.5e4,0.,0.,0.,0.,0.,0.,
	    1.5e4,15010.,0.,0.,0.,0.,0.,0.,15010.,15020.,0.,0.,0.,0.,0.,0.,
	    15020.,15030.,0.,0.,0.,0.,0.,0.,15030.,15040.,0.,0.,0.,0.,0.,0.,
	    15040.,15050.,0.,0.,0.,0.,0.,0.,15050.,15060.,0.,0.,0.,0.,0.,0.,
	    15060.,15070.,0.,0.,0.,0.,0.,0.,15070.,15080.,0.,0.,0.,0.,0.,0.,
	    15080.,15090.,0.,0.,0.,0.,0.,0.,15090.,15100.,0.,0.,0.,0.,0.,0.,
	    15100.,15110.,0.,0.,0.,0.,0.,0.,15110.,15120.,0.,0.,0.,0.,0.,0.,
	    15120.,15130.,0.,0.,0.,0.,0.,0.,15130.,15140.,0.,0.,0.,0.,0.,0.,
	    15140.,15150.,0.,0.,0.,0.,0.,0.,15150.,15160.,0.,0.,0.,0.,0.,0.,
	    15160.,15170.,0.,0.,0.,0.,0.,0.,15170.,15180.,0.,0.,0.,0.,0.,0.,
	    15180.,15190.,0.,0.,0.,0.,0.,0.,15190.,15200.,0.,0.,0.,0.,0.,0.,
	    15200.,15210.,0.,0.,0.,0.,0.,0.,15210.,15220.,0.,0.,0.,0.,0.,0.,
	    15220.,15230.,0.,0.,0.,0.,0.,0.,15230.,15240.,0.,0.,0.,0.,0.,0.,
	    15240.,15250.,0.,0.,0.,0.,0.,0.,15250.,15260.,0.,0.,0.,0.,0.,0.,
	    15260.,15270.,0.,0.,0.,0.,0.,0.,15270.,15280.,0.,0.,0.,0.,0.,0.,
	    15280.,15290.,0.,0.,0.,0.,0.,0.,15290.,15300. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     methane (12740 - 15290 cm-1) */

/*<    >*/
    /* Parameter adjustments */
    --a;

    /* Function Body */
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/

/*<       do i=1,8 >*/
    for (i__ = 1; i__ <= 8; ++i__) {
/*<       a(i)=acr(i,inu) >*/
	a[i__] = acr[i__ + (*inu << 3) - 9];
/*<       enddo >*/
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* meth5_ */

#ifdef __cplusplus
	}
#endif
