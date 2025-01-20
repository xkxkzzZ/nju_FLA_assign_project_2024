
#Q = {ca,cb,qa,qb,f,er,wi1,wl1,wl2,we,wg,wa,wl3,ws,wi2,wn,wp,wu,wt}
; copya,copyb,qa,qb,finish,error,write

#S = {a,b}
#G = {a,b,c,x,y,_,i,l,e,g,n,p,u,t}
#q0 = ca
#B = _
#F = {f}
#N = 2

; a->x, b->y, write c
; back, y->b, find a (-> x) 

ca a_ _a rr ca
ca b_ _b rr cb
ca __ __ *l qa
cb b_ _b rr cb
cb __ __ *l qa
cb a_ _a rr er

qa _b _b *l qa
qa _y _b *l qa
qa _x _x *l qa
qa _a _x *r qb
qb _x _x *r qb
qb _b cy rr qb
qb __ __ *l qa
qa __ __ l* f

er a_ _a rr er
er b_ _b rr er
er __ __ ** wi1
wi1 __ i_ r* wl1
wl1 __ l_ r* wl2
wl2 __ l_ r* we
we __ e_ r* wg
wg __ g_ r* wa 
wa __ a_ r* wl3
wl3 __ l_ r* ws 
ws __ __ r* wi2
wi2 __ i_ r* wn
wn __ n_ r* wp
wp __ p_ r* wu 
wu __ u_ r* wt 
wt __ t_ ** f