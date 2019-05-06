%        Date: 2019/3/19
%        Name: shengchang Gao
%        ID: 6519754323
%        Email: shengchg@usc.edu
function [leading_vector] = find_25(G)
%get the input name of the file and return the 25-D characteristic vector 
%% creat the filter bank
L5 = [1 4 6 4 1];
E5 = [-1 -2 0 2 1];
S5 = [-1 0 2 0 -1];
W5 = [-1 2 0 -2 1];
R5 = [1 -4 6 -4 1];
% creat the filter las's filter bank 
B(:,:,1) = (E5.')*L5;
B(:,:,2) = (S5.')*L5;
B(:,:,3) = (W5.')*L5;
B(:,:,4) = (R5.')*L5;
B(:,:,5) = (L5.')*L5;

B(:,:,6) = (L5.')*E5;
B(:,:,7)= (E5.')*E5;
B(:,:,8) = (S5.')*E5;
B(:,:,9) = (W5.')*E5;
B(:,:,10) = (R5.')*E5;

B(:,:,11)= (L5.')*S5;
B(:,:,12) = (E5.')*S5;
B(:,:,13) = (S5.')*S5;
B(:,:,14) = (W5.')*S5;
B(:,:,15) = (R5.')*S5;

B(:,:,16) = (L5.')*R5;
B(:,:,17) = (E5.')*R5;
B(:,:,18) = (S5.')*R5;
B(:,:,19) = (W5.')*R5;
B(:,:,20) = (R5.')*R5;

B(:,:,21) = (L5.')*S5;
B(:,:,22) = (E5.')*S5;
B(:,:,23) = (S5.')*S5;
B(:,:,24) = (W5.')*S5;
B(:,:,25) = (R5.')*S5;
%% reduce the illumination effects
G_total = 0;                %get the total pixel value

for i = 1:128
    for j = 1:128
        G_total = G(i,j)+G_total;
    end
end

G_mean = G_total/(128*128);

for i = 1:128
    for j = 1:128
        G_new(i,j) = G(i,j) - G_mean;
    end
end

%% do the image extension with reflection extension
I = zeros(132,132);
for i = 1:128
    for j = 1:128
        I(i+2,j+2) = G_new(i,j);
    end
end
%extend the rows
for j = 3:130
    I(1,j) = G_new(3,j-2);
    I(2,j) = G_new(2,j-2);
    I(131,j) = G_new(127,j-2);
    I(132,j) = G_new(126,j-2);
end
%extend the columns
for i = 3:130
    I(i,1) = G_new(i-2,3);
    I(i,2) = G_new(i-2,2);
    I(i,131) = G_new(i-2,127);
    I(i,132) = G_new(i-2,126);
end
%extend four coners
I(1,1) = G_new(3,3);
I(2,2) = G_new(2,2);
I(1,2) = G_new(3,2);
I(2,1) = G_new(2,3);

I(132,1) = G_new(126,3);
I(131,2) = G_new(127,2);
I(132,2) = G_new(126,2);
I(131,1) = G_new(127,3);

I(1,132) = G_new(3,126);
I(2,131) = G_new(2,127);
I(1,131) = G_new(3,127);
I(2,132) = G_new(2,126);

I(132,132) = G_new(127,127);
I(131,131) = G_new(126,126);
I(132,131) = G_new(127,126);
I(131,132) = G_new(126,127);
%% apply the matirx into image
for i = 3:130
    for j = 3:130
        for k = 1:25
         Sum = 0;   
         matrix = [I(i-2,j-2),I(i-2,j-1),I(i-2,j),I(i-2,j+1),I(i-2,j+2);
                  I(i-1,j-2),I(i-1,j-1),I(i-1,j),I(i-1,j+1),I(i-1,j+2);
                  I(i,j-2),I(i,j-1),I(i,j),I(i,j+1),I(i,j+2);
                  I(i+1,j-2),I(i+1,j-1),I(i+1,j),I(i+1,j+1),I(i+1,j+2);
                  I(i+2,j-2),I(i+2,j-1),I(i+2,j),I(i+2,j+1),I(i+2,j+2)];
         filter = B(:,:,k);
         Value = matrix.*filter;
         % calculate the pixel feature vector
         for q = 1:5
             for p = 1:5
                 Sum = Value(q,p) + Sum;
              end
         end
          F_v(i-2,j-2,k) = Sum;
        end
    end
end
%% average to get the 25-D leading vector
for k = 1:25
    All_sum = 0;
    for i = 1:128
        for j = 1:128
            All_sum = (F_v(i,j,k)^2)+All_sum;
        end
    end
    Leading_V(k) = sqrt(All_sum/(128*128));
end
leading_vector = Leading_V;
end
        
