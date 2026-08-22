/*
 * abb.cpp
 *
 *  Created on: Sep 2, 2022
 *      Author: jsaavedr
 */

#include "/home/edauandes/eda_cpp/trees_tarea3/avl/include/trees/avl.hpp"
#include <iostream>


namespace trees {

AVL::AVL():root(nullptr) {
	// TODO Auto-generated constructor stub
}

void AVL::balance(AVLNode* node){
	RotationType rType = getRotationType(node);

	if (rType == RotationType::LeftRotation){
		leftRotation(node);
	}
	else if (rType == RotationType::RightRotation){
		rightRotation(node);
	}
	else if (rType == RotationType::LeftRightRotation){
		leftRightRotation(node);
	}
	else if (rType == RotationType::RightLeftRotation){
		rightLeftRotation(node);
	}

}

RotationType AVL::getRotationType(AVLNode* node){
	/*this will run if node is unbalanced*/
	RotationType rType = RotationType::LeftRightRotation;
	AVLNode* leftChild = node->getLeft();
	AVLNode* rightChild = node->getRight();
	if (node->getLeftHeight() > node->getRightHeight()){
		if (leftChild->getLeftHeight() > leftChild->getRightHeight()){
			//unbalance left-left
			rType = RotationType::RightRotation;
		}
		else{
			//unbalance right-left  (from deeper node to)
			rType = RotationType::LeftRightRotation;
		}
	}
	else{ //RIGHT
		if (rightChild->getLeftHeight() > rightChild->getRightHeight()){
			//unbalance left-right
			rType = RotationType::RightLeftRotation;
		}
		else{
			//unbalance right-right
			rType = RotationType::LeftRotation;
		}
	}
	return rType;
}

void AVL::leftRotation(AVLNode* node){

	AVLNode* rightChild = node->getRight();
	AVLNode* parent = node->getParent();
	bool isLeft = node->isLeft();
	node->setRight(rightChild->getLeft());
	rightChild->setLeft(node);
	node->updateChildrenHeights();
	rightChild->updateChildrenHeights();
	if (node == root){
		root = rightChild;
	}
	else{
		if (isLeft){
			parent->setLeft(rightChild);
		}
		else{
			parent->setRight(rightChild);
		}
		parent->updateChildrenHeights();
	}
}

void AVL::leftRightRotation(AVLNode* node){

	leftRotation(node->getLeft());
	rightRotation(node);
}

void AVL::rightRotation(AVLNode* node){

	AVLNode* leftChild = node->getLeft();
	AVLNode* parent = node->getParent();
	bool isLeft = node->isLeft();
	node->setLeft(leftChild->getRight());
	leftChild->setRight(node);
	node->updateChildrenHeights();
	leftChild->updateChildrenHeights();

	if (node == root){
		root = leftChild;
	}
	else{
		if (isLeft){
			parent->setLeft(leftChild);
		}
		else{
			parent->setRight(leftChild);
		}
		parent->updateChildrenHeights();
	}
}

void AVL::rightLeftRotation(AVLNode* node){

	rightRotation(node->getRight());
	leftRotation(node);
}

void AVL::insert_rec(int val, AVLNode* node){

	if (val < node->getData()){
		if (node->getLeft() == nullptr){
			node->setLeft(new AVLNode(val, node));
		}
		else{
			insert_rec(val, node->getLeft());
		}
	}
	else{
		if (node->getRight() == nullptr){
			node->setRight(new AVLNode(val, node));
		}
		else{
			insert_rec(val, node->getRight());
		}
	}

	node->updateChildrenHeights();
	if (node->getBalanceScore() > 1){
		balance(node);
	}

	//std::cout<<std::flush;

}

void AVL::insert(int val){
	if (root == nullptr){
		root = new AVLNode(val);
	}
	else{
		insert_rec(val, root);
	}
}

AVLNode* AVL::find_rec(int val, AVLNode* node) {
    // Verifica si el nodo actual es nulo (caso base para cuando no se encuentra el valor)
    if (node == nullptr) {
        return nullptr;
    }

    // Si encontramos el valor, retornamos el nodo actual
    if (node->getData() == val) {
        return node;
    }

    // Recursión hacia la izquierda o derecha dependiendo del valor
    if (val < node->getData()) {
        return find_rec(val, node->getLeft());
    } else {
        return find_rec(val, node->getRight());
    }
}

AVLNode* AVL::find(int val) {
    return find_rec(val, root);
}

void AVL::traverse_rec(AVLNode* node, int label){
	if (node != nullptr){
		for (int i = 0; i < label; i++){
			std::cout << "*" << std::flush;
		}
		char T = node->isLeft()?'L':'R';
		std::cout << node->getData() << "  " << T <<std::endl;
		traverse_rec(node->getLeft(), label + 1);
		traverse_rec(node->getRight(), label + 1);
	}
}

void AVL::traverse(){
	traverse_rec(root, 1);
}


AVL::~AVL() {
	delete root;
}

} /* namespace trees */
