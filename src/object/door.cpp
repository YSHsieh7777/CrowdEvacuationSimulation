#include "object/door.hpp"

Door::Door(float l_bound, float r_bound , float u_bound, float d_bound, bool go_outside):
            m_l_bound(l_bound), m_r_bound(r_bound), m_u_bound(u_bound), m_d_bound(d_bound),
            m_go_outside(go_outside)
{}

const float & Door::l_bound() const { return m_l_bound; }
const float & Door::r_bound() const { return m_r_bound; }
const float & Door::u_bound() const { return m_u_bound; }
const float & Door::d_bound() const { return m_d_bound; }
const bool & Door::go_outside() const { return m_go_outside; }