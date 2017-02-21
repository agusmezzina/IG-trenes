model MovBezier
  Real p[3];
  Real v[3];
  Real a[3];
  Real alpha;
  Real u;
  Real b;
  parameter Real p0[3] = {0,0,0};
  parameter Real p1[3] = {25,0,50};
  parameter Real p2[3] = {50,0,60};
  parameter Real p3[3] = {75,0,0};
  parameter Real pi = 3.14159265358979;
 equation
  u = time/10;  
  p = Bezier(u,p0,p1,p2,p3);
  v = BezierDer(u,p0,p1,p2,p3)/10;
  a = BezierDer2(u,p0,p1,p2,p3)/100;
  b = sqrt(p*p) * sqrt(v*v);
  
  if (b > 0) then
    alpha = acos((p*v)/b) * 180 / pi;
  else
    alpha = 0;
  end if;
end MovBezier;