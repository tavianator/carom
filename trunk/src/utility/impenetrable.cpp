#include <carom.hpp>

namespace carom
{
  // Elastic collision response
  void collision(body& b1, body& b2) {
    impenetrable* ib1 = dynamic_cast<impenetrable*>(&b1);
    impenetrable* ib2 = dynamic_cast<impenetrable*>(&b2);

    if (ib1 != 0) {
      vector_displacement o1 = ib1->surface().center();

      for (body::iterator i = b2.begin(); i != b2.end(); ++i) {
        mesh::iterator j = ib1->surface().inside(o1, i->s());

        if (j != ib1->surface().end() && dot(i->s() - o1, i->v()) > 0) {
          scalar_mass     m1 = (j->a->m() + j->b->m() + j->c->m())/3;
          scalar_mass     m2 = i->m();
          vector_momentum p1 = (j->a->p() + j->b->p() + j->c->p())/3;
          vector_momentum p2 = i->p();

          vector_momentum I1 = 2*(m1*p2 - m2*p1)/(m1 + m2);
          vector_momentum I2 = 2*(m2*p2 - m1*p2)/(m1 + m2);

          ib1->collision(j, 3*I1);
          i->p(i->p() + I2);
        }
      }
    }

    if (ib2 != 0) {
      vector_displacement o2 = ib2->surface().center();

      for (body::iterator i = b1.begin(); i != b1.end(); ++i) {
        mesh::iterator j = ib2->surface().inside(o2, i->s());

        if (j != ib2->surface().end() && dot(i->s() - o2, i->v()) > 0) {
          scalar_mass     m1 = i->m();
          scalar_mass     m2 = (j->a->m() + j->b->m() + j->c->m())/3;
          vector_momentum p1 = i->p();
          vector_momentum p2 = (j->a->p() + j->b->p() + j->c->p())/3;

          vector_momentum I1 = 2*(m1*p2 - m2*p1)/(m1 + m2);
          vector_momentum I2 = 2*(m2*p2 - m1*p2)/(m1 + m2);

          i->p(i->p() + I1);
          ib2->collision(j, 3*I2);
        }
      }
    }
  }
}
