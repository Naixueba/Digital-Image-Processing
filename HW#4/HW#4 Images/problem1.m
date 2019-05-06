%    	Date: 2019/3/19
%    	Name: shengchang Gao
%    	ID: 6519754323
%    	Email: shengchg@usc.edu
%% get the 25-D vector features for each input image
G1 = readraw('texture1.raw');
vector(:,:,1) = find_25(G1);
G2 = readraw('texture2.raw');
vector(:,:,2) = find_25(G2);
G3 = readraw('texture3.raw');
vector(:,:,3) = find_25(G3);
G4 = readraw('texture4.raw');
vector(:,:,4) = find_25(G4);
G5 = readraw('texture5.raw');
vector(:,:,5) = find_25(G5);
G6 = readraw('texture6.raw');
vector(:,:,6) = find_25(G6);
G7 = readraw('texture7.raw');
vector(:,:,7) = find_25(G7);
G8 = readraw('texture8.raw');
vector(:,:,8) = find_25(G8);
G9 = readraw('texture9.raw');
vector(:,:,9) = find_25(G9);
G10 = readraw('texture10.raw');
vector(:,:,10) = find_25(G10);
G11 = readraw('texture11.raw');
vector(:,:,11) = find_25(G11);
G12 = readraw('texture12.raw');
vector(:,:,12) = find_25(G12);
%% Apply PCA to reduece 3-D feature vector?finish down from 25 vectors to 3 vectors
% get the feature data set
P = [vector(:,:,1);vector(:,:,2);vector(:,:,3);vector(:,:,4);vector(:,:,5);vector(:,:,6);
     vector(:,:,7);vector(:,:,8);vector(:,:,9);vector(:,:,10);vector(:,:,11);vector(:,:,12)];
p_T = P.';
% normoalization the parameters
p_T1 = nom(p_T);
p_1 = p_T1.';
% apply the PCA to reduce the parameters
[coffe,score,latent] = pca(p_T1);
tranMatrix = coffe(:,1:3);
New_V = (coffe(:,1:3).')*p_T1;
%% k-mean to the 25-D adn 3-D vector
idx_25 = kmeans(p_1,4);
idx_3 = kmeans(New_V.',4);