/*
 * abb.cpp
 *
 *  Created on: Sep 2, 2022
 *      Author: jsaavedr
 */

#include "/home/edauandes/eda_cpp/trees_tarea3/rb_tree/include/trees/rb.hpp"
#include <iostream>


namespace trees {

RB::RB():root(nullptr) {}

void RB::balance(RBNode* node) {
    switch (getRotationType(node)) {
        case case_1: do_case_1(node); break;
        case case_2: do_case_2(node); break;
        case case_3: do_case_3(node); break;
        case case_4: do_case_4(node); break;
        case case_5: do_case_5(node); break;
        case case_6: do_case_6(node); break;
    }
    root->setColor(NodeColor::BLACK);
}

RotationType RB::getRotationType(RBNode* node) {
    if (node->getColor() == 'B') return RotationType::case_1;

    char brother = (node->isLeft() && node->getParent()->getRight()) ? 
                   node->getParent()->getRight()->getColor() : 
                   (node->isRight() && node->getParent()->getLeft()) ? 
                   node->getParent()->getLeft()->getColor() : 'B';

    if (node->getColor() == 'R') {
        if (brother == 'R') return RotationType::case_2;
        if (node->isLeft()) {
            if (node->getLeft() && node->getLeft()->getColor() == 'R') 
                return RotationType::case_3;
            if (node->getRight() && node->getRight()->getColor() == 'R') 
                return RotationType::case_4;
        } else {
            if (node->getRight() && node->getRight()->getColor() == 'R') 
                return RotationType::case_5;
            if (node->getLeft() && node->getLeft()->getColor() == 'R') 
                return RotationType::case_6;
        }
    }

    return RotationType::case_1;
}


void RB::rotRight(RBNode* node) {
    if (!node || !node->getParent()) return;
    //std::cout << "ROT RIGHT" << std::endl;

    RBNode* parent = node->getParent();
    if (parent == root) {
        root = node;
        root->setParent(nullptr);
    } else {
        if (parent->isLeft()) {
            parent->getParent()->setLeft(node);
        } else {
            parent->getParent()->setRight(node);
        }
    }
    
    parent->setLeft(node->getRight());
    if (node->getRight()) {
        node->getRight()->setParent(parent);
    }
    node->setParent(parent->getParent());
    parent->setParent(node);
    node->setRight(parent);
}

void RB::rotLeft(RBNode* node) {
    if (!node || !node->getParent()) return;
    //std::cout << "ROT LEFT" << std::endl;

    RBNode* parent = node->getParent();
    if (parent == root) {
        root = node;
        root->setParent(nullptr);
    } else {
        if (parent->isLeft()) {
            parent->getParent()->setLeft(node);
        } else {
            parent->getParent()->setRight(node);
        }
    }
    parent->setRight(node->getLeft());
    if (node->getLeft()) {
        node->getLeft()->setParent(parent);
    }
    node->setParent(parent->getParent());
    parent->setParent(node);
    node->setLeft(parent);
}

void RB::do_case_1(RBNode* node) {
    ;
}

void RB::do_case_2(RBNode* node) {
    if (!node || !node->getParent()) return;
    node->setColor(NodeColor::BLACK);
    node->getParent()->setColor(NodeColor::RED);
    if (node->isLeft() && node->getParent()->getRight()) {
        node->getParent()->getRight()->setColor(NodeColor::BLACK);
    }
    if (node->isRight() && node->getParent()->getLeft()) {
        node->getParent()->getLeft()->setColor(NodeColor::BLACK);
    }
}

void RB::do_case_3(RBNode* node){
    rotRight(node);
    node->setColor(NodeColor::BLACK);
    node->getRight()->setColor(NodeColor::RED);
}

void RB::do_case_4(RBNode* node){
    
    rotLeft(node->getRight());
    rotRight(node->getParent());
    node=node->getParent();
    node->setColor(NodeColor::BLACK);
    node->getRight()->setColor(NodeColor::RED);
    
}
void RB::do_case_5(RBNode* node){
    rotLeft(node);
    node->setColor(NodeColor::BLACK);
    node->getLeft()->setColor(NodeColor::RED);
    
}

void RB::do_case_6(RBNode* node){
    rotRight(node->getLeft());
    rotLeft(node->getParent());
    node=node->getParent();
    node->setColor(NodeColor::BLACK);
    node->getLeft()->setColor(NodeColor::RED);
}

void RB::insert(int val, RBNode* node){
    if (val < node->getData()){
        if (node->getLeft() == nullptr){
            node->setLeft(new RBNode(val, node));
        }
        else{
            
            insert(val, node->getLeft());
        }
    }
    else{
        if (node->getRight() == nullptr){
            node->setRight(new RBNode(val, node));
        }
        else{
            insert(val, node->getRight());
        }
    }
    bool right_is_red =false;
    if(node->getRight()!=nullptr){
        if(node->getRight()->getColor()=='R')
            right_is_red=true;
    }

    bool left_is_red =false;
    if(node->getLeft()!=nullptr){
        if(node->getLeft()->getColor()=='R')
            left_is_red=true;
    }
    
    if(node->getParent()!=nullptr){
        if((left_is_red)||(right_is_red)){
        balance(node);
        }  
    }
}

void RB::insert(int val){
    if (root == nullptr){
        root = new RBNode(val);
    }
    else{
        insert(val, root);
    }
}

RBNode* RB::find_rec(int val, RBNode* node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->getData() == val) {
        return node;
    }
    if (val < node->getData()) {
        return find_rec(val, node->getLeft());
    } else {
        return find_rec(val, node->getRight());
    }
}

RBNode* RB::find(int val) {
    return find_rec(val, root);
}

void RB::traverse(RBNode* node, int label){
    if (node != nullptr){
        for (int i = 0; i < label; i++){
            std::cout << "*" << std::flush;
        }
        char T = node->isLeft()?'L':'R';
        std::cout << node->getData() << "  " << T << node->getColor() <<std::endl;
        traverse(node->getLeft(), label + 1);
        traverse(node->getRight(), label + 1);
    }
}

void RB::traverse(){
    traverse(root, 1);
}


RB::~RB() {
    delete root;
}

} /* namespace trees */

