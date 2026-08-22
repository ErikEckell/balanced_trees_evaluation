/*
 * abb.hpp
 *
 *  Created on: Sep 2, 2022
 *      Author: jsaavedr
 */

#ifndef RB_HPP_
#define RB_HPP_

#include "/home/edauandes/eda_cpp/trees_tarea3/rb_tree/include/trees/rbNode.hpp"

namespace trees {

enum RotationType{
    case_1 = 10,
    case_2 = 20,
    case_3 = 30,
    case_4 = 40,
    case_5 = 50,
    case_6 = 60,
};

class RB {
private:
    RBNode* root;
    void balance(RBNode* node);
    void do_case_1(RBNode* node);
    void do_case_2(RBNode* node);
    void do_case_3(RBNode* node);
    void do_case_4(RBNode* node);
    void do_case_5(RBNode* node);
    void do_case_6(RBNode* node);
    void rotRight(RBNode* node);
    void rotLeft(RBNode* node);
    void rot(RBNode* node, bool left);
    RotationType getRotationType(RBNode* node);
public:
    RB();
    void insert(int val, RBNode* node);
    void insert(int val);
    RBNode* find_rec(int val, RBNode* node);
    RBNode* find(int val);
    void traverse(RBNode* node, int level);
    void traverse();
    virtual ~RB();
};

} /* namespace trees */

#endif /* RB_HPP_ */