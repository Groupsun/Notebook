# Matrices and Systems of Linear Equations

## Introduction to Matrices and Systems of Linear Equations

In general a **linear equation** in n unkonws is an equation that cant be put in the form:

$$ a_1x_1 + a_2x_2 + ... + a_nx_n = b $$

The above equation is called linear because each term has degree one in the variables $ x_1, x_2, ..., x_n $.

### Linear Systems

In general, an **(m x n) system of linear equations** is a set of equations of the form:

$ a_{11}x_1 + a_{12}x_2 + ... + a_{1n}x_n = b_1 $
$ a_{21}x_1 + a_{22}x_2 + ... + a_{2n}x_n = b_2 $
... ...
$ a_{m1}x_1 + a_{m2}x_2 + ... + a_{mn}x_n = b_m$

A **solution** to system above is a sequence $ s_1, ..., s_n $ of numbers that is simultaneously a solution for each equation in the system.

### Geometric Interpretations of Solution Sets

**Remark** An (m x n) system of linear equations has either infinitely many solutions, no solutions, or a unique solution.

In general, a system of equations is called **consistent**(相容) if it has at least one solution, and the system is called inconsistent if it has no solution.

### Matrices

Matrix theory provides a convenient and natural symbolic language to describe linear systems and an appropriate and powerful framework within which to analyze and solve more general linear problems.

An **(m x n) matrix** is a rectangular array of numbers of the form:

$$      A =
        \begin{bmatrix}
        a_{11} & a_{12} & \cdots & a_{1n} \\
        a_{21} & a_{22} & \cdots & a_{2n} \\
        \vdots & \vdots & & \vdots \\
        a_{m1} & a_{m2} & \cdots & a_{mn}
        \end{bmatrix}
$$

The subscripts for the entry $a_{ij}$ indicate that the number appears in the $i$th row and $j$th column of A.

### Matrix Representation of a Linear System

In general, with the (m x n) system of linear equations

$ a_{11}x_1 + a_{12}x_2 + ... + a_{1n}x_n = b_1 $
$ a_{21}x_1 + a_{22}x_2 + ... + a_{2n}x_n = b_2 $
... ...
$ a_{m1}x_1 + a_{m2}x_2 + ... + a_{mn}x_n = b_m$

we associate two matrices:

The **coefficient matrix** for system above is the (m x n) matrix A where

$$      A =
        \begin{bmatrix}
        a_{11} & a_{12} & \cdots & a_{1n} \\
        a_{21} & a_{22} & \cdots & a_{2n} \\
        \vdots & \vdots & & \vdots \\
        a_{m1} & a_{m2} & \cdots & a_{mn}
        \end{bmatrix}
$$

The **argumented matrix** for system above is the [m x (n+1)] matrix B where

$$      B = 
        \begin{bmatrix}
        a_{11} & a_{12} & \cdots & a_{1n} & b_1 \\
        a_{21} & a_{22} & \cdots & a_{2n} & b_2 \\
        \vdots & \vdots & & \vdots & \vdots\\
        a_{m1} & a_{m2} & \cdots & a_{mn} & b_m
        \end{bmatrix}
$$

### Elementary Operations

There are two steps involved in solving an (m x n) system of equations. The steps are:

1. Reduction of the system(that is, the elimination of variables).
2. Description of the set of solutions.

#### Definition 1

Two systems of linear equations in n unkonws are equivalent provided that they have the same set of solutions.

#### Theorem 1

If one of the following elementary operations is applied to a system of linear equations, then the resulting system is equivalent to the original system.

1. Interchange two equations.
2. Multiply an equation by a nonzero scalar.
3. Add a constant multiple of one equation to another.

### Row Operations

#### Definition 2

The following operations, performed on the row of a matrix, are called elementary row operations:

1. Interchange two rows.
2. Multiply a row by nonzero scalar.
3. Add a constant multiple of one row to another.


