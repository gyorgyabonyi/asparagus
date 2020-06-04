// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CONFIG_H
#define ASPARAGUS_CONFIG_H

#include <string>

namespace asparagus {

class Config final {
public:
    Config();

    constexpr bool use_gomocup_protocol() const { return use_gomocup_protocol_; }
    constexpr bool is_exact_five() const { return is_exact_five_; }
    constexpr int max_depth() const { return max_depth_; }

    void Load(int argc, char** argv);
    int Get(const std::string& key) const;
    void Set(const std::string& key, int value);

private:
    bool use_gomocup_protocol_;
    bool is_exact_five_;
    int max_depth_;
};

}  // namespace asparagus

#endif  // ASPARAGUS_CONFIG_H
