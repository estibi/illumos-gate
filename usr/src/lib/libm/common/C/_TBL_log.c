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

#include "libm_protos.h"

/*
 * Table of constants for log, log2, and log10
 * By K.C. Ng, November 21, 2004
 *
 * Y[j], 1/Y[j], log(Y[j]) for j = 0 to 255
 * where HIWORD(Y[j]) ~ 0x3fb8400 + (j<<15)
 * That is, 256 Y[j] space out logrithmically between 0.09375 and 24, and
 * each is chosen so that 1/Y[j] and log(Y[j]) are very close to a IEEE
 * double. In addition, each log(Y[j]) has 3 trailing zeros.
 */
const double _TBL_log[] = {
9.47265623608246343e-02, 1.05567010464380857e+01, -2.35676082856530300e+00,
9.66796869131412717e-02, 1.03434344062203838e+01, -2.33635196153499791e+00,
9.86328118117651004e-02, 1.01386139321308306e+01, -2.31635129573594156e+00,
1.00585936733578435e-01, 9.94174764856737347e+00, -2.29674282498938709e+00,
1.02539062499949152e-01, 9.75238095238578850e+00, -2.27751145544242561e+00,
1.04492186859904843e-01, 9.57009351656812157e+00, -2.25864297726331742e+00,
1.06445312294918631e-01, 9.39449543094380957e+00, -2.24012392529694537e+00,
1.08398437050250693e-01, 9.22522526350104144e+00, -2.22194160843615762e+00,
1.10351562442130582e-01, 9.06194690740703912e+00, -2.20408398741152212e+00,
1.12304686894746625e-01, 8.90434787407592943e+00, -2.18653968262558962e+00,
1.14257811990227776e-01, 8.75213679118525256e+00, -2.16929787526329321e+00,
1.16210936696872255e-01, 8.60504207627572093e+00, -2.15234831939887172e+00,
1.18164061975360682e-01, 8.46280995492959498e+00, -2.13568126444263484e+00,
1.20117187499996322e-01, 8.32520325203277523e+00, -2.11928745022706622e+00,
1.22070312499895098e-01, 8.19200000000703987e+00, -2.10315806829801133e+00,
1.24023436774175100e-01, 8.06299217317146599e+00, -2.08728472499318229e+00,
1.26953123746900931e-01, 7.87692315467275872e+00, -2.06393736501443570e+00,
1.30859374098123454e-01, 7.64179109744297769e+00, -2.03363201254049386e+00,
1.34765623780674720e-01, 7.42028992220936967e+00, -2.00421812948999545e+00,
1.38671874242985771e-01, 7.21126764500034501e+00, -1.97564475345722457e+00,
1.42578124148616536e-01, 7.01369867201821506e+00, -1.94786518986246371e+00,
1.46484374166731490e-01, 6.82666670549979404e+00, -1.92083651719164372e+00,
1.50390624434435488e-01, 6.64935067435644189e+00, -1.89451920694646070e+00,
1.54296874339723084e-01, 6.48101268596180624e+00, -1.86887677685174936e+00,
1.58203124999987427e-01, 6.32098765432149001e+00, -1.84387547036714849e+00,
1.62109374999815342e-01, 6.16867469880220742e+00, -1.81948401724404896e+00,
1.66015624243955634e-01, 6.02352943919619310e+00, -1.79567337310324682e+00,
1.69921874302298687e-01, 5.88505749542848644e+00, -1.77241651049093640e+00,
1.73828124315277527e-01, 5.75280901142480605e+00, -1.74968825924644555e+00,
1.77734374286237506e-01, 5.62637364896854919e+00, -1.72746512253855222e+00,
1.81640624146994889e-01, 5.50537636993989743e+00, -1.70572513658236602e+00,
1.85546874316304788e-01, 5.38947370406942916e+00, -1.68444773712372431e+00,
1.89453124405085355e-01, 5.27835053203882509e+00, -1.66361364967629299e+00,
1.93359374570531595e-01, 5.17171718320401652e+00, -1.64320477712600699e+00,
1.97265624263334577e-01, 5.06930694962380368e+00, -1.62320411193263148e+00,
2.01171874086291030e-01, 4.97087380898513764e+00, -1.60359564135180399e+00,
2.05078123979995308e-01, 4.87619050044336610e+00, -1.58436427985572159e+00,
2.08984373896073439e-01, 4.78504675424820736e+00, -1.56549579585994181e+00,
2.12890623963011144e-01, 4.69724772930228163e+00, -1.54697674768135762e+00,
2.16796874723889421e-01, 4.61261261848719517e+00, -1.52879442500076479e+00,
2.20703124198150608e-01, 4.53097346778917753e+00, -1.51093680996032553e+00,
2.24609374375627030e-01, 4.45217392541970725e+00, -1.49339249945607477e+00,
2.28515625000094036e-01, 4.37606837606657528e+00, -1.47615069024134016e+00,
2.32421873924349737e-01, 4.30252102831546246e+00, -1.45920113655598627e+00,
2.36328123935216378e-01, 4.23140497774241098e+00, -1.44253408394829741e+00,
2.40234375000066919e-01, 4.16260162601510064e+00, -1.42614026966681173e+00,
2.44140623863132178e-01, 4.09600001907347711e+00, -1.41001089239381727e+00,
2.48046874999894917e-01, 4.03149606299383390e+00, -1.39413753858134015e+00,
2.53906248590769879e-01, 3.93846156032078243e+00, -1.37079018013412401e+00,
2.61718748558906533e-01, 3.82089554342693294e+00, -1.34048483059486401e+00,
2.69531249159214337e-01, 3.71014493910979404e+00, -1.31107094300173976e+00,
2.77343749428383191e-01, 3.60563381024826013e+00, -1.28249756949928795e+00,
2.85156249289339359e-01, 3.50684932380819214e+00, -1.25471800582335113e+00,
2.92968749999700462e-01, 3.41333333333682321e+00, -1.22768933094427446e+00,
3.00781248554318814e-01, 3.32467534065511261e+00, -1.20137202743229921e+00,
3.08593748521894806e-01, 3.24050634463533127e+00, -1.17572959680235023e+00,
3.16406249999639899e-01, 3.16049382716409077e+00, -1.15072828980826181e+00,
3.24218749999785061e-01, 3.08433734939963511e+00, -1.12633683668362750e+00,
3.32031248841858584e-01, 3.01176471638753718e+00, -1.10252619147729547e+00,
3.39843749265406558e-01, 2.94252874199264314e+00, -1.07926932798654107e+00,
3.47656249999834799e-01, 2.87640449438338930e+00, -1.05654107474789782e+00,
3.55468749999899247e-01, 2.81318681318761055e+00, -1.03431793796299587e+00,
3.63281249999864997e-01, 2.75268817204403371e+00, -1.01257795132667816e+00,
3.71093749064121570e-01, 2.69473684890124421e+00, -9.91300555400967731e-01,
3.78906249999751032e-01, 2.63917525773369288e+00, -9.70466465976836723e-01,
3.86718748879039009e-01, 2.58585859335407608e+00, -9.50057597243619156e-01,
3.94531249999987899e-01, 2.53465346534661240e+00, -9.30056927638333697e-01,
4.02343749999485523e-01, 2.48543689320706163e+00, -9.10448456251205407e-01,
4.10156249578856991e-01, 2.43809524059864202e+00, -8.91217095348825872e-01,
4.17968749447214571e-01, 2.39252336765021800e+00, -8.72348611340208357e-01,
4.25781248601723117e-01, 2.34862386092395203e+00, -8.53829565534445223e-01,
4.33593749393073047e-01, 2.30630630953458038e+00, -8.35647244566987801e-01,
4.41406248572254134e-01, 2.26548673299152270e+00, -8.17789629001761220e-01,
4.49218749348472501e-01, 2.22608695975035964e+00, -8.00245317566669279e-01,
4.57031249277175089e-01, 2.18803419149470768e+00, -7.83003511263371976e-01,
4.64843748529596368e-01, 2.15126051100659366e+00, -7.66053954531254355e-01,
4.72656248830947701e-01, 2.11570248457175136e+00, -7.49386901356188240e-01,
4.80468748609962581e-01, 2.08130081902951236e+00, -7.32993092000230995e-01,
4.88281249241778237e-01, 2.04800000318021258e+00, -7.16863708730099525e-01,
4.96093748931098810e-01, 2.01574803583926521e+00, -7.00990360175606675e-01,
5.07812497779701388e-01, 1.96923077784079825e+00, -6.77642998396260410e-01,
5.23437498033319737e-01, 1.91044776837204044e+00, -6.47337648285891021e-01,
5.39062498006593560e-01, 1.85507247062801328e+00, -6.17923763020271188e-01,
5.54687498964024250e-01, 1.80281690477552603e+00, -5.89350388745976339e-01,
5.70312499806522322e-01, 1.75342465812909332e+00, -5.61570823110474571e-01,
5.85937497921867001e-01, 1.70666667271966777e+00, -5.34542153929987052e-01,
6.01562498226483444e-01, 1.66233766723853860e+00, -5.08224845014116688e-01,
6.17187498682654212e-01, 1.62025316801528496e+00, -4.82582413587029357e-01,
6.32812500000264566e-01, 1.58024691357958624e+00, -4.57581109246760320e-01,
6.48437499353274216e-01, 1.54216867623689291e+00, -4.33189657120379490e-01,
6.64062498728508976e-01, 1.50588235582451335e+00, -4.09379009344016609e-01,
6.79687498865382267e-01, 1.47126437027210688e+00, -3.86122146934356092e-01,
6.95312498728747119e-01, 1.43820224982050338e+00, -3.63393896015796081e-01,
7.10937499999943157e-01, 1.40659340659351906e+00, -3.41170757402847080e-01,
7.26562499999845568e-01, 1.37634408602179792e+00, -3.19430770766573779e-01,
7.42187500000120126e-01, 1.34736842105241350e+00, -2.98153372318914478e-01,
7.57812499999581890e-01, 1.31958762886670744e+00, -2.77319285416786077e-01,
7.73437498602746576e-01, 1.29292929526503420e+00, -2.56910415591577124e-01,
7.89062500000142664e-01, 1.26732673267303819e+00, -2.36909747078176913e-01,
8.04687500000259015e-01, 1.24271844660154174e+00, -2.17301275689659512e-01,
8.20312499999677036e-01, 1.21904761904809900e+00, -1.98069913762487504e-01,
8.35937499999997113e-01, 1.19626168224299478e+00, -1.79201429457714445e-01,
8.51562499999758749e-01, 1.17431192660583728e+00, -1.60682381690756770e-01,
8.67187500000204725e-01, 1.15315315315288092e+00, -1.42500062607046951e-01,
8.82812500000407896e-01, 1.13274336283133503e+00, -1.24642445206814556e-01,
8.98437499999816813e-01, 1.11304347826109651e+00, -1.07098135556570995e-01,
9.14062499999708455e-01, 1.09401709401744296e+00, -8.98563291221800009e-02,
9.29687500000063949e-01, 1.07563025210076635e+00, -7.29067708080189947e-02,
9.45312499999844014e-01, 1.05785123966959604e+00, -5.62397183230410880e-02,
9.60937500000120459e-01, 1.04065040650393459e+00, -3.98459085470743157e-02,
9.76562499999976685e-01, 1.02400000000002445e+00, -2.37165266173399170e-02,
9.92187500000169420e-01, 1.00787401574785940e+00, -7.84317746085513856e-03,
1.01562500000004907e+00, 9.84615384615337041e-01,  1.55041865360135717e-02,
1.04687500000009237e+00, 9.55223880596930641e-01,  4.58095360313824362e-02,
1.07812500000002154e+00, 9.27536231884039442e-01,  7.52234212376075018e-02,
1.10937499999982481e+00, 9.01408450704367703e-01,  1.03796793681485644e-01,
1.14062500000007416e+00, 8.76712328767066285e-01,  1.31576357788784293e-01,
1.17187500000009659e+00, 8.53333333333263000e-01,  1.58605030176721007e-01,
1.20312499999950173e+00, 8.31168831169175393e-01,  1.84922338493597849e-01,
1.23437500000022027e+00, 8.10126582278336449e-01,  2.10564769107528083e-01,
1.26562500000064615e+00, 7.90123456789720069e-01,  2.35566071313277448e-01,
1.29687500000144706e+00, 7.71084337348537208e-01,  2.59957524438041876e-01,
1.32812499999945932e+00, 7.52941176470894757e-01,  2.83768173130237500e-01,
1.35937500055846350e+00, 7.35632183605830825e-01,  3.07025035705735583e-01,
1.39062499999999467e+00, 7.19101123595508374e-01,  3.29753286372464149e-01,
1.42187500000017564e+00, 7.03296703296616421e-01,  3.51976423157301710e-01,
1.45312500161088876e+00, 6.88172042247866766e-01,  3.73716410902152685e-01,
1.48437500134602307e+00, 6.73684209915422660e-01,  3.94993809147663466e-01,
1.51562499999932343e+00, 6.59793814433284220e-01,  4.15827895143264570e-01,
1.54687500000028200e+00, 6.46464646464528614e-01,  4.36236766775100371e-01,
1.57812500000061906e+00, 6.33663366336385092e-01,  4.56237433481979870e-01,
1.60937500243255216e+00, 6.21359222361793417e-01,  4.75845906381452632e-01,
1.64062500000026312e+00, 6.09523809523711768e-01,  4.95077266798011895e-01,
1.67187500000027911e+00, 5.98130841121395473e-01,  5.13945751102401260e-01,
1.70312500224662178e+00, 5.87155962528224662e-01,  5.32464800188589216e-01,
1.73437500283893620e+00, 5.76576575632799071e-01,  5.50647119589526390e-01,
1.76562500399259092e+00, 5.66371680135198341e-01,  5.68504737613959144e-01,
1.79687500443862880e+00, 5.56521737755718449e-01,  5.86049047473771623e-01,
1.82812500114411280e+00, 5.47008546666207462e-01,  6.03290852063923744e-01,
1.85937500250667465e+00, 5.37815125325376786e-01,  6.20240411099985067e-01,
1.89062500504214515e+00, 5.28925618424108568e-01,  6.36907464903988974e-01,
1.92187500371610143e+00, 5.20325202245941476e-01,  6.53301273946326866e-01,
1.95312500494870611e+00, 5.11999998702726389e-01,  6.69430656476366792e-01,
1.98437500351688123e+00, 5.03937006980894941e-01,  6.85304004871206018e-01,
2.03125000000003997e+00, 4.92307692307682621e-01,  7.08651367095930240e-01,
2.09375000579615866e+00, 4.77611938976327366e-01,  7.38956719359554093e-01,
2.15625000000061062e+00, 4.63768115941897652e-01,  7.68370601797816022e-01,
2.21875000323311955e+00, 4.50704224695355204e-01,  7.96943975698769513e-01,
2.28125000853738547e+00, 4.38356162743050726e-01,  8.24723542091080120e-01,
2.34374999999916556e+00, 4.26666666666818573e-01,  8.51752210736227866e-01,
2.40625000438447856e+00, 4.15584414827170512e-01,  8.78069520876078258e-01,
2.46875000884389584e+00, 4.05063289688167072e-01,  9.03711953249632494e-01,
2.53124999999940403e+00, 3.95061728395154743e-01,  9.28713251872476775e-01,
2.59375000434366632e+00, 3.85542168029044230e-01,  9.53104706671537905e-01,
2.65625000734081196e+00, 3.76470587194880080e-01,  9.76915356454189698e-01,
2.71875000787161980e+00, 3.67816090889081959e-01,  1.00017221875016560e+00,
2.78125001557333462e+00, 3.59550559784484969e-01,  1.02290047253181449e+00,
2.84375001147093220e+00, 3.51648350229895601e-01,  1.04512360775085789e+00,
2.90625000771072894e+00, 3.44086020592463127e-01,  1.06686359300668343e+00,
2.96875001371853831e+00, 3.36842103706616824e-01,  1.08814099342179560e+00,
3.03125000512624965e+00, 3.29896906658595002e-01,  1.10897507739479018e+00,
3.09375001373132807e+00, 3.23232321797685962e-01,  1.12938395177327244e+00,
3.15625001204422961e+00, 3.16831681959289180e-01,  1.14938461785752644e+00,
3.21875000888250318e+00, 3.10679610793130057e-01,  1.16899308818952186e+00,
3.28125000000102052e+00, 3.04761904761809976e-01,  1.18822444735810784e+00,
3.34375001587649123e+00, 2.99065419140752298e-01,  1.20709293641028914e+00,
3.40625000791328070e+00, 2.93577980969346064e-01,  1.22561198175258212e+00,
3.46875000615970519e+00, 2.88288287776354346e-01,  1.24379430028837845e+00,
3.53125000516822674e+00, 2.83185840293502689e-01,  1.26165191737618265e+00,
3.59375001425228779e+00, 2.78260868461675415e-01,  1.27919622952937750e+00,
3.65625001719730669e+00, 2.73504272217836075e-01,  1.29643803670156643e+00,
3.71875000856489324e+00, 2.68907562405871714e-01,  1.31338759261496740e+00,
3.78125001788371806e+00, 2.64462808666557803e-01,  1.33005464752659286e+00,
3.84375001532508964e+00, 2.60162600588744020e-01,  1.34644845655970613e+00,
3.90625000429340918e+00, 2.55999999718627136e-01,  1.36257783560168733e+00,
3.96875001912740766e+00, 2.51968502722644594e-01,  1.37845118847836900e+00,
4.06250002536431332e+00, 2.46153844616978895e-01,  1.40179855389937913e+00,
4.18750001743208244e+00, 2.38805969155131859e-01,  1.43210390131407017e+00,
4.31250002253733200e+00, 2.31884056759177282e-01,  1.46151778758352613e+00,
4.43750000671406397e+00, 2.25352112335092170e-01,  1.49009115631456268e+00,
4.56250002627485340e+00, 2.19178080929562313e-01,  1.51787072466748185e+00,
4.68750001185115028e+00, 2.13333332793974317e-01,  1.54489939382477459e+00,
4.81250001682742301e+00, 2.07792207065640028e-01,  1.57121670311050998e+00,
4.93750000000042366e+00, 2.02531645569602875e-01,  1.59685913022732606e+00,
5.06249999999927613e+00, 1.97530864197559108e-01,  1.62186043243251454e+00,
5.18750002327641901e+00, 1.92771083472381588e-01,  1.64625189004383721e+00,
5.31250002381002329e+00, 1.88235293273997795e-01,  1.67006253873242194e+00,
5.43750000000577405e+00, 1.83908045976816203e-01,  1.69331939641586438e+00,
5.56250002193114934e+00, 1.79775280190080267e-01,  1.71604765143503712e+00,
5.68749999999938005e+00, 1.75824175824194989e-01,  1.73827078427695980e+00,
5.81250002749782002e+00, 1.72043009938785768e-01,  1.76001077564428243e+00,
5.93749999999874767e+00, 1.68421052631614471e-01,  1.78128816936054868e+00,
6.06250001966917473e+00, 1.64948453073088669e-01,  1.80212225950800153e+00,
6.18750003004243609e+00, 1.61616160831459688e-01,  1.82253113275015188e+00,
6.31250002448351388e+00, 1.58415840969730465e-01,  1.84253179848005466e+00,
6.43750001359968849e+00, 1.55339805497076044e-01,  1.86214026810242750e+00,
6.56250003345742350e+00, 1.52380951604072529e-01,  1.88137163301601618e+00,
6.68750002403557531e+00, 1.49532709742937614e-01,  1.90024011581622965e+00,
6.81250003423489581e+00, 1.46788990088028509e-01,  1.91875916501466826e+00,
6.93750003062940923e+00, 1.44144143507740546e-01,  1.93694148348760287e+00,
7.06250002747386052e+00, 1.41592919803171097e-01,  1.95479910036266347e+00,
7.18750003617887856e+00, 1.39130434082284093e-01,  1.97234341115705192e+00,
7.31250000000050537e+00, 1.36752136752127301e-01,  1.98958521255804399e+00,
7.43750002212249761e+00, 1.34453781112678528e-01,  2.00653477384620160e+00,
7.56250003604752941e+00, 1.32231404328381430e-01,  2.02320182812357530e+00,
7.68750005007207449e+00, 1.30081299965731312e-01,  2.03959563964607682e+00,
7.81249996125652668e+00, 1.28000000634773070e-01,  2.05572501010335529e+00,
7.93750005224239974e+00, 1.25984251139310915e-01,  2.07159837080052966e+00,
8.12500004244456164e+00, 1.23076922433975874e-01,  2.09494573343974722e+00,
8.37500006149772425e+00, 1.19402984197849338e-01,  2.12525108505414195e+00,
8.62500006593247370e+00, 1.15942028099206410e-01,  2.15466497056176820e+00,
8.87500007743793873e+00, 1.12676055354884341e-01,  2.18323834408688100e+00,
9.12500001754142609e+00, 1.09589040885222130e-01,  2.21101790139090326e+00,
9.37500007707016181e+00, 1.06666665789779500e-01,  2.23804658007729174e+00,
9.62500004426353151e+00, 1.03896103418305616e-01,  2.26436388477265638e+00,
9.87500006518495788e+00, 1.01265822116353585e-01,  2.29000631738819393e+00,
1.01250000000026539e+01, 9.87654320987395445e-02,  2.31500761299286495e+00,
1.03750000409819823e+01, 9.63855417879450060e-02,  2.33939907006683256e+00,
1.06250000362555337e+01, 9.41176467376672460e-02,  2.36320971822276604e+00,
1.08750000879032314e+01, 9.19540222452362582e-02,  2.38646658505780351e+00,
1.11250000697274576e+01, 8.98876398860551373e-02,  2.40919483431994053e+00,
1.13750000462194141e+01, 8.79120875548795450e-02,  2.43141796890025930e+00,
1.16250000714972366e+01, 8.60215048472860316e-02,  2.45315795762371991e+00,
1.18750000788855150e+01, 8.42105257563797310e-02,  2.47443535656369562e+00,
1.21250000895724916e+01, 8.24742261948517991e-02,  2.49526944421096886e+00,
1.23750000985058719e+01, 8.08080801648427965e-02,  2.51567831641482442e+00,
1.26250000894226950e+01, 7.92079202310506381e-02,  2.53567898224440924e+00,
1.28750000768594433e+01, 7.76699024489580225e-02,  2.55528745251946532e+00,
1.31250000578007420e+01, 7.61904758549435401e-02,  2.57451881288155349e+00,
1.33750000809310077e+01, 7.47663546877819496e-02,  2.59338729883298669e+00,
1.36250000915049636e+01, 7.33944949199294983e-02,  2.61190634726526838e+00,
1.38750000830616607e+01, 7.20720716406179490e-02,  2.63008866561892418e+00,
1.41249999999960103e+01, 7.07964601770111474e-02,  2.64794627703222218e+00,
1.43750000290097564e+01, 6.95652172509168693e-02,  2.66549058870148414e+00,
1.46250000868097665e+01, 6.83760679702078294e-02,  2.68273239905363070e+00,
1.48750000966053975e+01, 6.72268903196987927e-02,  2.69968195792617394e+00,
1.51250001097012756e+01, 6.61157019998031836e-02,  2.71634901116988203e+00,
1.53750000510427132e+01, 6.50406501905787804e-02,  2.73274281701243282e+00,
1.56250001080665442e+01, 6.39999995573594382e-02,  2.74887220253872400e+00,
1.58750000434989929e+01, 6.29921258116476201e-02,  2.76474554751884938e+00,
1.62500000641781739e+01, 6.15384612954199342e-02,  2.78809291272517257e+00,
1.67500001015987401e+01, 5.97014921751882754e-02,  2.81839826433667184e+00,
1.72500001048300184e+01, 5.79710141404578272e-02,  2.84781214955447126e+00,
1.77500001262529885e+01, 5.63380277682904579e-02,  2.87638552303426920e+00,
1.82500001543340602e+01, 5.47945200845665337e-02,  2.90416508848516131e+00,
1.87500001096404212e+01, 5.33333330214672482e-02,  2.93119375826390893e+00,
1.92500001680268191e+01, 5.19480514946147609e-02,  2.95751106946245912e+00,
1.97500000329124035e+01, 5.06329113080278073e-02,  2.98315349301358168e+00,
2.02500001270002485e+01, 4.93827157396732261e-02,  3.00815479982416534e+00,
2.07500001519906796e+01, 4.81927707313324349e-02,  3.03254625400155930e+00,
2.12500001425219267e+01, 4.70588232137922752e-02,  3.05635690207734001e+00,
2.17500000758314478e+01, 4.59770113339538697e-02,  3.07961376102119644e+00,
2.22500001767207358e+01, 4.49438198677525880e-02,  3.10234201655475417e+00,
2.27500001365873317e+01, 4.39560436921389575e-02,  3.12456515140079816e+00,
2.32500001697599998e+01, 4.30107523741288036e-02,  3.14630513933487066e+00,
2.37500001766865303e+01, 4.21052628446554611e-02,  3.16758253792008304e+00,
};
