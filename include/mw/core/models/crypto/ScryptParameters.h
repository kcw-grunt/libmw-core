#pragma once

#include <mw/core/traits/Jsonable.h>
#include <cstdint>

class ScryptParameters : public Traits::IJsonable
{
public:
    uint32_t N;
    uint32_t r;
    uint32_t p;

    ScryptParameters(const uint32_t N_, const uint32_t r_, const uint32_t p_) : N(N_), r(r_), p(p_) { }
    virtual ~ScryptParameters() = default;

    virtual json ToJSON() const override final
    {
        return json({
            {"N", N},
            {"r", r},
            {"p", p}
        });
    }

    static ScryptParameters FromJSON(const json& json)
    {
        return ScryptParameters(
            json["N"].get<uint32_t>(),
            json["r"].get<uint32_t>(),
            json["p"].get<uint32_t>()
        );
    }
};