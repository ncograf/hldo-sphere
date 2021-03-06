Point(1) = {-1.0, -1.0, 0, 1.0};
Point(2) = { 0.0, -1.0, 0, 1.0};
Point(3) = { 0.0,  0.0, 0, 1.0};
Point(4) = { 1.0,  0.0, 0, 1.0};
Point(5) = { 1.0,  1.0, 0, 1.0};
Point(6) = {-1.0,  1.0, 0, 1.0};

Characteristic Length {6, 5, 4, 3, 2, 1} = 0.4;

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 1};

Line Loop(1) = {1, 2, 3, 4, 5, 6};

Plane Surface(1) = {1};
