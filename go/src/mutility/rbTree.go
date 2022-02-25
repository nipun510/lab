package mutility

import (
	"fmt"
)

type COLOR int

const (
  RED COLOR = iota
  BLACK
)

type Data int

type Node struct {
  data Data
  color COLOR
  left, right, parent *Node
}


type RbTree struct{
  root *Node
};

func (this *RbTree) insertBST(currNode, node *Node) {
  if currNode.data < node.data {
    if currNode.right != nil {
      this.insertBST(currNode.right, node)
    } else {
      currNode.right = node
      currNode.right.parent = currNode
    }
  } else {
    if currNode.left != nil {
      this.insertBST(currNode.left, node)
    } else {
      currNode.left = node
      currNode.left.parent = currNode
    }
  }
}

func (this *RbTree) rotateLeft(node *Node) {
  node_right := node.right
  node.right = node_right.left
  if node.right != nil {
    node.right.parent = node
  }
  node_right.parent = node.parent
  if node.parent == nil {
    this.root = node_right
  } else if node.parent.left == node {
    node.parent.left = node_right
  } else {
    node.parent.right = node_right
  }
  node_right.left = node
  node.parent = node_right
}

func (this *RbTree) rotateRight(node *Node) {
  node_left := node.left
  node.left = node_left.right
  if node.left != nil {
    node.left.parent = node
  }
  node_left.parent = node.parent
  if node.parent == nil {
    this.root = node_left
  } else if node.parent.left == node {
    node.parent.left = node_left
  } else {
    node.parent.right = node_left
  }
  node_left.right = node
  node.parent = node_left
}

func (this *RbTree) sucessor (node *Node) (*Node) {
  tmp := node.right
  for tmp.left != nil {
    tmp = tmp.left
  }
  return tmp
}

func (this *RbTree) BSTReplace(node *Node) (*Node) {
  if node.left == nil && node.right == nil {
    return nil
  }
  if node.left == nil {
    return node.right
  } else if node.right == nil {
    return node.left
  } else {
    return this.sucessor(node)
  }
}

func (this *RbTree) removeVal(data Data) (bool){
  node := this.find(data)
  if node == nil {
    return false
  } else {
    this.remove(node)
    return true
  }
}

func (this *RbTree) remove(v *Node) {
  u := this.BSTReplace(v)
  uvBlack := (u == nil || u.color == BLACK) && v.color == BLACK
  //case when v is leaf node
  if v.left == nil && v.right == nil {
    if v == this.root {
      this.root = nil
      return
    } else {
      if uvBlack {
        this.fixDoubleBlack(v)
      }
      if v.parent.left == v {
        v.parent.left = nil
      } else {
        v.parent.right = nil
      }
      return
    }
  }
  //case when v has exactly 1 non-null child
  if v.left == nil || v.right == nil {
    if v == this.root {
      this.root = u
      u.parent = nil
    } else {
      parent := v.parent
      if parent.left == v {
        parent.left = u
      } else {
        parent.right = u
      }
      u.parent = parent
      if uvBlack {
        this.fixDoubleBlack(u)
      } else {
        u.color = BLACK
      }
    }
    return
  }
  //case when both children of v are not null
  u.data, v.data = v.data, u.data
  this.remove(u)
}

func (this *RbTree) inorderUtil(currNode *Node) {
  if currNode == nil {
    return
  }
  this.inorderUtil(currNode.left)
  fmt.Printf("{%d %d}\n", currNode.data, currNode.color)
  this.inorderUtil(currNode.right)
}

func (this *RbTree) inorder() {
  this.inorderUtil(this.root)
}

func (this *RbTree) findUtil(node *Node, data Data) (*Node) {
  if node == nil {
    return nil
  }
  if node.data < data {
    return this.findUtil(node.right, data)
  } else if node.data  > data {
    return this.findUtil(node.left, data)
  } else {
    return node
  }
}

func (this *RbTree) find(data Data) (*Node) {
  return this.findUtil(this.root, data)

}

func isBlack (node *Node) (bool) {
  return node == nil || node.color == BLACK
}

func (this *RbTree) fixDoubleBlack(node *Node) {

  if node == this.root {
    return
  }
  parent := node.parent
  sibling := &Node{}
  if parent.left == node {
    sibling = parent.right
  } else {
    sibling = parent.left
  }
  //sibling can't be nil because of double black
  if isBlack(sibling) {
    if isBlack(sibling.left) && isBlack(sibling.right) {
      sibling.color = RED
      this.fixDoubleBlack(parent)
      return
    } else {
      if sibling == parent.left {
        if sibling.left != nil && sibling.left.color == RED {
          // LL case
          sibling.left.color = sibling.color
          sibling.color = parent.color
          this.rotateRight(parent)
        } else {
          // LR case
          sibling.right.color = parent.color
          this.rotateLeft(sibling)
          this.rotateRight(parent)
        }
      } else {
        if sibling.right != nil && sibling.right.color == RED {
          // RR case
          sibling.right.color = sibling.color
          sibling.color = parent.color
          this.rotateLeft(parent)
        } else {
          // RL case
          sibling.left.color = parent.color
          this.rotateRight(sibling)
          this.rotateLeft(parent)
        }
      }
      parent.color = BLACK
      return
    }
  }

  if sibling.color == RED {
    parent.color = RED
    sibling.color = BLACK
    if sibling.parent.left == sibling {
      this.rotateRight(parent)
    } else {
      this.rotateLeft(parent)
    }
    this.fixDoubleBlack(node)
  }
  return

}

func (this *RbTree) fixViolation(node *Node) {
  parent := &Node{}
  grandParent := &Node{}
  for node != this.root && node.color == RED && node.parent.color == RED {
     parent = node.parent;
     grandParent = parent.parent
     if  parent == grandParent.left {
       uncle := grandParent.right
       // if uncle is red, only recoloring is required
       if uncle != nil && uncle.color == RED {
         grandParent.color = RED
         uncle.color = BLACK
         parent.color = BLACK
         node = grandParent
       } else {
         if node == parent.right {
           fmt.Printf("left rotate %d\n", parent.data)
           this.rotateLeft(parent)
           node, parent = parent, node
         }
         fmt.Printf("right rotate %d\n", grandParent.data)
         this.rotateRight(grandParent)
         parent.color, grandParent.color = grandParent.color, parent.color
         node = parent
       }
    } else {
      uncle := grandParent.left
      //if uncle is red only recoloring is required
      if uncle != nil && uncle.color == RED {
        grandParent.color = RED
        uncle.color = BLACK
        parent.color = BLACK
        node = grandParent
      } else {
        if node == parent.left {
          fmt.Printf("right rotate %d\n", parent.data)
          this.rotateRight(parent)
          parent, node = node, parent
        }
        fmt.Printf("left rotate %d\n", grandParent.data)
        this.rotateLeft(grandParent)
        parent.color, grandParent.color = grandParent.color, parent.color
        node = parent
      }
    }
  }
  this.root.color = BLACK

}

func (this *RbTree) insert(data Data) {
  if this.root == nil {
    node := &Node{data, BLACK, nil, nil, nil}
    this.root = node
    this.root.color = BLACK
    return
  }
  node := &Node{data, RED, nil, nil, nil}
  this.insertBST(this.root, node)
  this.fixViolation(node)
}

func (this *RbTree) contains(data Data) (bool) {
  return this.find(data) != nil
}


func TestRBTree (){
  rbTree := RbTree{nil}
  rbTree.insert(11)
	rbTree.insert(18)
	rbTree.insert(10)

	rbTree.insert(43)
  rbTree.inorder()
	fmt.Printf("contains val %d %t\n", 18, rbTree.contains(18))
  rbTree.removeVal(18)
  rbTree.removeVal(10)
  rbTree.inorder()
	fmt.Printf("contains val %d %t\n", 18, rbTree.contains(18))
}
