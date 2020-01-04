class Door
{
public:
    Door(float l_bound, float r_bound , float u_bound, float d_bound, bool go_outside):
            m_l_bound(l_bound), m_r_bound(r_bound), m_u_bound(u_bound), m_d_bound(d_bound),
            m_go_outside(go_outside)
    {}
    ~Door() = default;

    const float & l_bound() const { return m_l_bound; }
    const float & r_bound() const { return m_r_bound; }
    const float & u_bound() const { return m_u_bound; }
    const float & d_bound() const { return m_d_bound; }
    const bool & go_outside() const { return m_go_outside; }

private:
    float m_l_bound, m_r_bound, m_u_bound, m_d_bound;
    bool m_go_outside;
};