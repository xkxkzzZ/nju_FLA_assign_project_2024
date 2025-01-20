#Q = {s,a,b,c,t,f,e,wt,wr,wu,we,wf,wa,wl,ws}
; start,add,back,check,true,false,end,write

#S = {1}
#G = {0,1,_,t,r,u,e,f,a,l,s}
#q0 = s
#B = _
#F = {e}
#N = 2

; n = 1, m = 1
; m += 2, n += m

s 1_ 01 rr c
c __ __ ll t
c 1_ 11 *r a
a 1_ 11 ** b
b 11 11 *l b
b 1_ 1_ *r c
c 11 01 rr c
c _1 _1 ll f

t 01 _1 l* t
t _1 _1 r* wt
wt _1 t1 r* wr
wr _1 r1 r* wu
wu _1 u1 r* we
we _1 e1 ** e

f 01 _1 l* f
f _1 _1 r* wf
wf _1 f1 r* wa
wa _1 a1 r* wl
wl _1 l1 r* ws
ws _1 s1 r* we