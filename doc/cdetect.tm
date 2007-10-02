<TeXmacs|1.0.6.10>

<style|article>

<\body>
  <doc-data|<doc-title|Collision Detection>|<\doc-subtitle>
    in the Carom Physics Engine
  </doc-subtitle>|<\doc-author-data|<author-name|Tavian Barnes>>
    \;
  </doc-author-data>>

  \;

  To test for the intersection of two triangles, it suffices to test for the
  intersections between the line segments of each triangle and the other
  triangle. To accomplish this, we define the plane containing a triangle
  given by points <math|<wide|p<rsub|0>|\<vect\>>>,
  <math|<wide|p<rsub|1>|\<vect\>>>, and <math|<wide|p<rsub|2>|\<vect\>>> by

  <\equation>
    u*<left|(><wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>+v*<left|(><wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>+<wide|p<rsub|0>|\<vect\>><label|plane>
  </equation>

  with <math|u> and <math|v> arbitrary parameters, and note that a point on
  the plane is within this triangle if and only if these conditions are met:

  <\eqnarray*>
    <tformat|<table|<row|<cell|u>|<cell|\<in\>>|<cell|<left|[>0,1<right|]>>>|<row|<cell|v>|<cell|\<in\>>|<cell|<left|[>0,1<right|]>>>|<row|<cell|u+v>|<cell|\<leq\>>|<cell|1>>>>
  </eqnarray*>

  \;

  Then, we define a line passing through two verticies
  <math|<wide|l<rsub|0>|\<vect\>>> and <math|<wide|l<rsub|1>|\<vect\>>> of
  the other triangle by

  <\equation>
    <wide|l<rsub|0>|\<vect\>>+t*(<wide|l<rsub|1>|\<vect\>>-<wide|l<rsub|0>|\<vect\>>)<label|line>
  </equation>

  and note that a point on this line is within the line segment bounded by
  <math|<wide|l<rsub|0>|\<vect\>>> and <math|<wide|l<rsub|1>|\<vect\>>> if
  and only if <math|t\<in\><left|[>0,1<right|]>>. To solve for <math|t>,
  <math|u>, and <math|v>, we set (<reference|plane>) equal to
  (<reference|line>),

  <\eqnarray*>
    <tformat|<table|<row|<cell|u*<left|(><wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>+v*<left|(><wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>+<wide|p<rsub|0>|\<vect\>>>|<cell|=>|<cell|<wide|l<rsub|0>|\<vect\>>+t*(<wide|l<rsub|1>|\<vect\>>-<wide|l<rsub|0>|\<vect\>>)>>|<row|<cell|t*(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)+u*<left|(><wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>+v*<left|(><wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>>|<cell|=>|<cell|<wide|l<rsub|0>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>>>>>
  </eqnarray*>

  and represent this using a matrix:

  <\equation*>
    <matrix|<tformat|<table|<row|<cell|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)>|<cell|<left|(><wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)>|<cell|<left|(><wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>><right|)>>>>>>*<matrix|<tformat|<table|<row|<cell|t>>|<row|<cell|u>>|<row|<cell|v>>>>>=<matrix|<tformat|<table|<row|<cell|<wide|l<rsub|0>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>>>>>>
  </equation*>

  \;

  We now use Cramer's rule to neatly solve this system, and find

  <\eqnarray*>
    <tformat|<table|<row|<cell|t>|<cell|=>|<cell|<frac|<det|<tformat|<table|<row|<cell|(<wide|l<rsub|0>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)>|<cell|(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)>|<cell|(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)>>>>>|<det|<tformat|<table|<row|<cell|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)>|<cell|(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)>|<cell|(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)>>>>>>>>|<row|<cell|>|<cell|=>|<cell|<frac|(<wide|l<rsub|0>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<cdot\>(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<times\>(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)\<cdot\>(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<times\>(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)><eq-number><label|t>>>>>
  </eqnarray*>

  where the scalar triple product has been used. Similarly,

  <\eqnarray*>
    <tformat|<table|<row|<cell|u>|<cell|=>|<cell|<frac|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)\<cdot\>(<wide|l<rsub|0>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<times\>(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)\<cdot\>(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<times\>(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)><eq-number><label|u>>>|<row|<cell|v>|<cell|=>|<cell|<frac|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)\<cdot\>(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<times\>(<wide|l<rsub|0>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)|(<wide|l<rsub|0>|\<vect\>>-<wide|l<rsub|1>|\<vect\>>)\<cdot\>(<wide|p<rsub|1>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)\<times\>(<wide|p<rsub|2>|\<vect\>>-<wide|p<rsub|0>|\<vect\>>)><eq-number><label|v>>>>>
  </eqnarray*>

  \;

  If all of the aforementioned conditions are met, i.e.,
  <math|t,u,v\<in\><left|[>0,1<right|]>> and
  <math|<left|(>u+v<right|)>\<leq\>1>, then a collision has taken place.
</body>

<\references>
  <\collection>
    <associate|line|<tuple|2|?>>
    <associate|plane|<tuple|1|?>>
    <associate|t|<tuple|3|?>>
    <associate|u|<tuple|4|?>>
    <associate|v|<tuple|5|?>>
  </collection>
</references>