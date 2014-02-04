/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * long double __k_lgammal(long double x, int *signgamlp);
 * K.C. Ng, August, 1989.
 *
 * We choose [1.5,2.5] to be the primary interval. Our algorithms
 * are mainly derived from
 *
 *
 *                             zeta(2)-1    2    zeta(3)-1    3
 * lgamma(2+s) = s*(1-euler) + --------- * s  -  --------- * s  + ...
 *                                 2                 3
 *
 *
 * Note 1. Since gamma(1+s)=s*gamma(s), hence
 *		lgamma(1+s) = log(s) + lgamma(s), or
 *		lgamma(s) = lgamma(1+s) - log(s).
 *	   When s is really tiny (like roundoff), lgamma(1+s) ~ s(1-enler)
 *	   Hence lgamma(s) ~ -log(s) for tiny s
 *
 */

#include "libm.h"
#include "libm_synonyms.h"
#include "longdouble.h"

static long double neg(long double, int *);
static long double poly(long double, const long double *, int);
static long double polytail(long double);
static long double primary(long double);

static const long double
c0 =	 0.0L,
ch =	 0.5L,
c1 =	 1.0L,
c2 =	 2.0L,
c3 =	 3.0L,
c4 =	 4.0L,
c5 =	 5.0L,
c6 =	 6.0L,
pi =	 3.1415926535897932384626433832795028841971L,
tiny =   1.0e-40L;

long double
__k_lgammal(long double x, int *signgamlp) {
	long double t, y;
	int i;

	/* purge off +-inf, NaN and negative arguments */
	if (!finitel(x))
		return (x*x);
	*signgamlp = 1;
	if (signbitl(x))
		return (neg(x, signgamlp));

	/* for x < 8.0 */
	if (x < 8.0L) {
	    y = anintl(x);
	    i = (int) y;
	    switch (i) {
	    case 0:
		if (x < 1.0e-40L)
			return (-logl(x));
		else
			return (primary(x)-log1pl(x))-logl(x);
	    case 1:
		return (primary(x-y)-logl(x));
	    case 2:
		return (primary(x-y));
	    case 3:
		return (primary(x-y)+logl(x-c1));
	    case 4:
		return (primary(x-y)+logl((x-c1)*(x-c2)));
	    case 5:
		return (primary(x-y)+logl((x-c1)*(x-c2)*(x-c3)));
	    case 6:
		return (primary(x-y)+logl((x-c1)*(x-c2)*(x-c3)*(x-c4)));
	    case 7:
		return (primary(x-y)+logl((x-c1)*(x-c2)*(x-c3)*(x-c4)*(x-c5)));
	    case 8:
		return primary(x-y)+
			logl((x-c1)*(x-c2)*(x-c3)*(x-c4)*(x-c5)*(x-c6));
	    }
	}

	/* 8.0 <= x < 1.0e40 */
	if (x < 1.0e40L) {
	    t = logl(x);
	    return (x*(t-c1)-(ch*t-polytail(c1/x)));
	}

	/* 1.0e40 <= x <= inf */
	return (x*(logl(x)-c1));
}

static const long double an1[] = {		/* 20 terms */
	-0.0772156649015328606065120900824024309741L,
	3.224670334241132182362075833230130289059e-0001L,
	-6.735230105319809513324605383668929964120e-0002L,
	2.058080842778454787900092432928910226297e-0002L,
	-7.385551028673985266273054086081102125704e-0003L,
	2.890510330741523285758867304409628648727e-0003L,
	-1.192753911703260976581414338096267498555e-0003L,
	5.096695247430424562831956662855697824035e-0004L,
	-2.231547584535777978926798502084300123638e-0004L,
	9.945751278186384670278268034322157947635e-0005L,
	-4.492623673665547726647838474125147631082e-0005L,
	2.050721280617796810096993154281561168706e-0005L,
	-9.439487785617396552092393234044767313568e-0006L,
	4.374872903516051510689234173139793159340e-0006L,
	-2.039156676413643091040459825776029327487e-0006L,
	9.555777181318621470466563543806211523634e-0007L,
	-4.468344919709630637558538313482398989638e-0007L,
	2.216738086090045781773004477831059444178e-0007L,
	-7.472783403418388455860445842543843485916e-0008L,
	8.777317930927149922056782132706238921648e-0008L,
};

static const long double an2[] = {		/* 20 terms */
  -.0772156649015328606062692723698127607018L,
   3.224670334241132182635552349060279118047e-0001L,
  -6.735230105319809367555642883133994818325e-0002L,
   2.058080842778459676880822202762143671813e-0002L,
  -7.385551028672828216011343150077846918930e-0003L,
   2.890510330762060607399561536905727853178e-0003L,
  -1.192753911419623262328187532759756368041e-0003L,
   5.096695278636456678258091134532258618614e-0004L,
  -2.231547306817535743052975194022893369135e-0004L,
   9.945771461633313282744264853986643877087e-0005L,
  -4.492503279458972037926876061257489481619e-0005L,
   2.051311416812082875492678651369394595613e-0005L,
  -9.415778282365955203915850761537462941165e-0006L,
   4.452428829045147098722932981088650055919e-0006L,
  -1.835024727987632579886951760650722695781e-0006L,
   1.379783080658545009579060714946381462565e-0006L,
   2.282637532109775156769736768748402175238e-0007L,
   1.002577375515900191362119718128149880168e-0006L,
   5.177028794262638311939991106423220002463e-0007L,
   3.127947245174847104122426445937830555755e-0007L,
};

static const long double an3[] = {		/* 20 terms */
  -.0772156649015328227870646417729220690875L,
   3.224670334241156699881788955959915250365e-0001L,
  -6.735230105312273571375431059744975563170e-0002L,
   2.058080842924464587662846071337083809005e-0002L,
  -7.385551008677271654723604653956131791619e-0003L,
   2.890510536479782086197110272583833176602e-0003L,
  -1.192752262076857692740571567808259138697e-0003L,
   5.096800771149805289371135155128380707889e-0004L,
  -2.231000836682831335505058492409860123647e-0004L,
   9.968912171073936803871803966360595275047e-0005L,
  -4.412020779327746243544387946167256187258e-0005L,
   2.281374113541454151067016632998630209049e-0005L,
  -4.028361291428629491824694655287954266830e-0006L,
   1.470694920619518924598956849226530750139e-0005L,
   1.381686137617987197975289545582377713772e-0005L,
   2.012493539265777728944759982054970441601e-0005L,
   1.723917864208965490251560644681933675799e-0005L,
   1.202954035243788300138608765425123713395e-0005L,
   5.079851887558623092776296577030850938146e-0006L,
   1.220657945824153751555138592006604026282e-0006L,
};

static const long double an4[] = {		/* 21 terms */
  -.0772156649015732285350261816697540392371L,
   3.224670334221752060691751340365212226097e-0001L,
  -6.735230109744009693977755991488196368279e-0002L,
   2.058080778913037626909954141611580783216e-0002L,
  -7.385557567931505621170483708950557506819e-0003L,
   2.890459838416254326340844289785254883436e-0003L,
  -1.193059036207136762877351596966718455737e-0003L,
   5.081914708100372836613371356529568937869e-0004L,
  -2.289855016133600313131553005982542045338e-0004L,
   8.053454537980585879620331053833498511491e-0005L,
  -9.574620532104845821243493405855672438998e-0005L,
  -9.269085628207107155601445001196317715686e-0005L,
  -2.183276779859490461716196344776208220180e-0004L,
  -3.134834305597571096452454999737269668868e-0004L,
  -3.973878894951937437018305986901392888619e-0004L,
  -3.953352414899222799161275564386488057119e-0004L,
  -3.136740932204038779362660900621212816511e-0004L,
  -1.884502253819634073946130825196078627664e-0004L,
  -8.192655799958926853585332542123631379301e-0005L,
  -2.292183750010571062891605074281744854436e-0005L,
  -3.223980628729716864927724265781406614294e-0006L,
};

static const long double ap1[] = {			/* 19 terms */
  -0.0772156649015328606065120900824024296961L,
   3.224670334241132182362075833230047956465e-0001L,
  -6.735230105319809513324605382963943777301e-0002L,
   2.058080842778454787900092126606252375465e-0002L,
  -7.385551028673985266272518231365020063941e-0003L,
   2.890510330741523285681704570797770736423e-0003L,
  -1.192753911703260971285304221165990244515e-0003L,
   5.096695247430420878696018188830886972245e-0004L,
  -2.231547584535654004647639737841526025095e-0004L,
   9.945751278137201960636098805852315982919e-0005L,
  -4.492623672777606053587919463929044226280e-0005L,
   2.050721258703289487603702670753053765201e-0005L,
  -9.439485626565616989352750672499008021041e-0006L,
   4.374838162403994645138200419356844574219e-0006L,
  -2.038979492862555348577006944451002161496e-0006L,
   9.536763152382263548086981191378885102802e-0007L,
  -4.426111214332434049863595231916564014913e-0007L,
   1.911148847512947464234633846270287546882e-0007L,
  -5.788673944861923038157839080272303519671e-0008L,
};

static const long double ap2[] = {			/* 19 terms */
  -0.077215664901532860606428624449354836087L,
   3.224670334241132182271948744265855440139e-0001L,
  -6.735230105319809467356126599005051676203e-0002L,
   2.058080842778453315716389815213496002588e-0002L,
  -7.385551028673653323064118422580096222959e-0003L,
   2.890510330735923572088003424849289006039e-0003L,
  -1.192753911629952368606185543945790688144e-0003L,
   5.096695239806718875364547587043220998766e-0004L,
  -2.231547520600616108991867127392089144886e-0004L,
   9.945746913898151120612322833059416008973e-0005L,
  -4.492599307461977003570224943054585729684e-0005L,
   2.050609891889165453592046505651759999090e-0005L,
  -9.435329866734193796540515247917165988579e-0006L,
   4.362267138522223236241016136585565144581e-0006L,
  -2.008556356653246579300491601497510230557e-0006L,
   8.961498103387207161105347118042844354395e-0007L,
  -3.614187228330216282235692806488341157741e-0007L,
   1.136978988247816860500420915014777753153e-0007L,
  -2.000532786387196664019286514899782691776e-0008L,
};

static const long double ap3[] = {			/* 19 terms */
  -0.077215664901532859888521470795348856446L,
   3.224670334241131733364048614484228443077e-0001L,
  -6.735230105319676541660495145259038151576e-0002L,
   2.058080842775975461837768839015444273830e-0002L,
  -7.385551028347615729728618066663566606906e-0003L,
   2.890510327517954083379032008643080256676e-0003L,
  -1.192753886919470728001821137439430882603e-0003L,
   5.096693728898932234814903769146577482912e-0004L,
  -2.231540055048827662528594010961874258037e-0004L,
   9.945446210018649311491619999438833843723e-0005L,
  -4.491608206598064519190236245753867697750e-0005L,
   2.047939071322271016498065052853746466669e-0005L,
  -9.376824046522786006677541036631536790762e-0006L,
   4.259329829498149111582277209189150127347e-0006L,
  -1.866064770421594266702176289764212873428e-0006L,
   7.462066721137579592928128104534957135669e-0007L,
  -2.483546217529077735074007138457678727371e-0007L,
   5.915166576378161473299324673649144297574e-0008L,
  -7.334139641706988966966252333759604701905e-0009L,
};

static const long double ap4[] = {			/* 19 terms */
  -0.0772156649015326785569313252637238673675L,
   3.224670334241051435008842685722468344822e-0001L,
  -6.735230105302832007479431772160948499254e-0002L,
   2.058080842553481183648529360967441889912e-0002L,
  -7.385551007602909242024706804659879199244e-0003L,
   2.890510182473907253939821312248303471206e-0003L,
  -1.192753098427856770847894497586825614450e-0003L,
   5.096659636418811568063339214203693550804e-0004L,
  -2.231421144004355691166194259675004483639e-0004L,
   9.942073842343832132754332881883387625136e-0005L,
  -4.483809261973204531263252655050701205397e-0005L,
   2.033260142610284888319116654931994447173e-0005L,
  -9.153539544026646699870528191410440585796e-0006L,
   3.988460469925482725894144688699584997971e-0006L,
  -1.609692980087029172567957221850825977621e-0006L,
   5.634916377249975825399706694496688803488e-0007L,
  -1.560065465929518563549083208482591437696e-0007L,
   2.961350193868935325526962209019387821584e-0008L,
  -2.834602215195368130104649234505033159842e-0009L,
};

static long double
primary(long double s) {	/* assume |s|<=0.5 */
	int i;

	i = (int) (8.0L * (s + 0.5L));
	switch (i) {
	case 0:	return ch*s+s*poly(s, an4, 21);
	case 1:	return ch*s+s*poly(s, an3, 20);
	case 2:	return ch*s+s*poly(s, an2, 20);
	case 3:	return ch*s+s*poly(s, an1, 20);
	case 4:	return ch*s+s*poly(s, ap1, 19);
	case 5:	return ch*s+s*poly(s, ap2, 19);
	case 6:	return ch*s+s*poly(s, ap3, 19);
	case 7:	return ch*s+s*poly(s, ap4, 19);
	}
	/* NOTREACHED */
    return (0.0L);
}

static long double
poly(long double s, const long double *p, int n) {
	long double y;
	int i;
	y = p[n-1];
	for (i = n-2; i >= 0; i--) y = p[i]+s*y;
	return (y);
}

static const long double pt[] = {
   9.189385332046727417803297364056176804663e-0001L,
   8.333333333333333333333333333331286969123e-0002L,
  -2.777777777777777777777777553194796036402e-0003L,
   7.936507936507936507927283071433584248176e-0004L,
  -5.952380952380952362351042163192634108297e-0004L,
   8.417508417508395661774286645578379460131e-0004L,
  -1.917526917525263651186066417934685675649e-0003L,
   6.410256409395203164659292973142293199083e-0003L,
  -2.955065327248303301763594514012418438188e-0002L,
   1.796442830099067542945998615411893822886e-0001L,
  -1.392413465829723742489974310411118662919e+0000L,
   1.339984238037267658352656597960492029261e+0001L,
  -1.564707657605373662425785904278645727813e+0002L,
   2.156323807499211356127813962223067079300e+0003L,
  -3.330486427626223184647299834137041307569e+0004L,
   5.235535072011889213611369254140123518699e+0005L,
  -7.258160984602220710491988573430212593080e+0006L,
   7.316526934569686459641438882340322673357e+0007L,
  -3.806450279064900548836571789284896711473e+0008L,
};

static long double
polytail(long double s) {
	long double t, z;
	int i;
	z = s*s;
	t = pt[18];
	for (i = 17; i >= 1; i--) t = pt[i]+z*t;
	return (pt[0]+s*t);
}

static long double
neg(long double z, int *signgamlp) {
	long double t, p;

	/*
	 * written by K.C. Ng,  Feb 2, 1989.
	 *
	 * Since
	 *		-z*G(-z)*G(z) = pi/sin(pi*z),
	 * we have
	 * 	G(-z) = -pi/(sin(pi*z)*G(z)*z)
	 *		  =  pi/(sin(pi*(-z))*G(z)*z)
	 * Algorithm
	 *		z = |z|
	 *		t = sinpi(z); ...note that when z>2**112, z is an int
	 *		and hence t=0.
	 *
	 *		if(t==0.0) return 1.0/0.0;
	 *		if(t< 0.0) *signgamlp = -1; else t= -t;
	 *		if(z<1.0e-40)	...tiny z
	 *		    return -log(z);
	 *		else
	 *		    return log(pi/(t*z))-lgamma(z);
	 *
	 */

	t = sinpil(z);			/* t := sin(pi*z) */
	if (t == c0)  			/* return   1.0/0.0 =  +INF */
	    return (c1/c0);

	z = -z;
	if (z <= tiny)
	    p = -logl(z);
	else
		p = logl(pi/(fabsl(t)*z)) - __k_lgammal(z, signgamlp);
	if (t < c0) *signgamlp = -1;
	return (p);
}
