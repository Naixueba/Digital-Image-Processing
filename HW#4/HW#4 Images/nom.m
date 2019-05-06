%    	Date: 2019/3/19
%    	Name: shengchang Gao
%    	ID: 6519754323
%    	Email: shengchg@usc.edu
function output = nom(p_T)
% normilaztion the parameters
 para_std = std(p_T);    %get the std of of the columns
 para_mean = mean(p_T);  %get the mean of the colunms
    for i = 1:12
       for j = 1:25
         new(j,i) = (p_T(j,i) - para_mean(i))/(para_std(i));
       end
    end
    output = new;
end