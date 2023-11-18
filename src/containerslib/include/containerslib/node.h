#ifndef _NODE_H_
#define _NODE_H_

typedef struct Node Node;

/**
 * @brief Construct a new Node 
 * 
 * @param value 
 * @param next 
 * @return Node* 
 * 
 * @note Complexity: O(1)
 */
Node *node_construct(void* value, Node *next);


/**
 * @brief Returns the data stored in the node.
 * 
 * @param n 
 * @return void* 
 * 
 * @note Complexity: O(1)
 */
void* node_get_value(Node *n);


/**
 * @brief Returns the pointer to the next node.
 * 
 * @param n 
 * @return Node* 
 * 
 * @note Complexity: O(1)
 */
Node *node_get_next(Node *n);


/**
 * @brief destroy a node
 * 
 * @param n 
 * @param next 
 * @return void* 
 * 
 * @note Complexity: O(1)
 */
void node_destroy(Node *n);


/**
 * @brief destroy a node recursively
 * 
 * @param n 
 * @param next 
 * @return void* 
 * 
 * @note Complexity: O(n)
 */
void node_destroy_recursive(Node* n);

#endif