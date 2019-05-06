% Demo for Structured Edge Detector (please see readme.txt first).

%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=0;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=4;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=0;                 % set to true to enable nms

%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
file_name = 'Pig.jpg';
I = imread(file_name);
tic, E=edgesDetect(I,model); toc
%figure(1); im(I); figure(2); im(1-E);title('SE Probability Map');

%% binarize
T = 0.08; E_binary = E;
[height, width] = size(E);
for i=1:height
    for j=1:width
        if ( E_binary(i,j) > T )
            E_binary(i,j)=0;
        else
            E_binary(i,j)=255;
        end
    end
end
figure(3); im(E_binary);
title(['SE Probability Map with Threshold P > ',num2str(T)]);

%% Performance Evaluation
G = 'Pig.mat'; G = load(G);
for i=1:5
    [thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg(E_binary,G.groundTruth{i}.Boundaries);
    %[thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg(E,'Tiger.mat');
    P_temp = cntP./(sumP+eps);
    R_temp = cntR./sumR;
    F_temp = 2*(P_temp.*R_temp)./(P_temp+R_temp+eps);
    [F_max, F_max_idx] = max(F_temp);
    p = P_temp(F_max_idx);
    r = R_temp(F_max_idx);
    P(i) = p;
    R(i) = r;
end

P_mean = mean(P)
R_mean = mean(R)
F_measure = 2*(P_mean * R_mean)/(P_mean + R_mean)
