% Test script for comparing fast L-1 solvers via Gaussian projections

% Copyright �2010. The Regents of the University of California (Regents). 
% All Rights Reserved. Contact The Office of Technology Licensing, 
% UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620, 
% (510) 643-7201, for commercial licensing opportunities.

% Authors: Arvind Ganesh, Allen Y. Yang, Zihan Zhou.
% Contact: Allen Y. Yang, Department of EECS, University of California,
% Berkeley. <yang@eecs.berkeley.edu>

% IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, 
% SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, 
% ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF 
% REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

% REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED
% TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
% PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, 
% PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO 
% PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

clear;
clc;
close all;

function [L,U] = LUdecomp(A);

 n = size(A,1);
 L = eye(n);

 for k=1:n-1
        for i=k+1:n
                L(i,k) = A(i,k)./A(k,k);

                for j=k:n
                        A(i,j)=A(i,j)-L(i,k).*A(k,j);
                end;
        end;
 end;

 U=A;
end;
function x = linsolve(A,b);

 [L,U]=LUdecomp(A);
 y = Lsolve(L,b);
 x= Usolve(U,y);

end;

#RandStream.setDefaultStream(RandStream('mt19937ar','seed',1));

DEBUG = 1;

addpath L1Solvers

%% Classicaly Interior-Point Methods
METHOD_OMP = 0;
% Orthogonal Matching Pursuit: Implementation adapted from SparseLab

METHOD_BP = 1;
% Scott Chen et al., Atomic Decomposition By Basis Pursuit.

%% Homotopy Methods
METHOD_HOMOTOPY = 2;
% Salman Asif et al., Dynamic Updating for L1 Minimization.
% David Donoho et al., Fast solution of L1-norm minimization problems when 
% the solution may be sparse.

METHOD_PFP = 3;
% Mark Plumbley, Recovery of Sparse Representations by Polytope Faces
% Pursuit.

METHOD_L1LS = 4;
% S. Kim et al., A Method for Large-Scale l1-Regularized Least Squares.

%% Iterative Thresholding Methods
METHOD_SpaRSA = 5;
% Stephen Wright et al., Sparse Reconstruction by Separable Approximation.

%% Nesterov's Methods
METHOD_FISTA = 6;
% Amir Beck et al., A Fast Iterative Shrinkage-Thresholding Algorithm 
% for Linear Inverse Problems

%% Alternating Direction Method
METHOD_ALM = 7;

%% Test parameters
l1Method = METHOD_FISTA;

STOPPING_GROUND_TRUTH = -1;
STOPPING_DUALITY_GAP = 1;
STOPPING_SPARSE_SUPPORT = 2;
STOPPING_OBJECTIVE_VALUE = 3;
STOPPING_SUBGRADIENT = 4;
stoppingCriterion = STOPPING_GROUND_TRUTH;

tryCount = 1;

n = 500;
noiseVariance = 0.01;
maxIteration = 5000;
isNonnegative = false;
lambda = 1e-2;
tolerance = 0.5;

dimensionIndex=0;
for d=300:200:1900

    for sparsityRatio=0.1
        
        dimensionIndex = dimensionIndex + 1;        
        k=ceil(sparsityRatio*n);
        
        currentRunTime = 0;
        currentIteration = 0;
        currentL2Error = 0;
        for tryIndex = 1:tryCount
            % Generate Gaussian dictionary
            AMatrix = randn(d,n);
            matrixNorm = AMatrix.'*AMatrix;
            matrixNorm = sqrt(diag(matrixNorm)).';
            AMatrix = AMatrix./repmat(matrixNorm, [d,1]);
            
            sparseSupport = randperm(n);
            x0=zeros(n,1);
            x0(sparseSupport(1:k))=randn(1,k);
            x0 = x0 / norm(x0);
            if isNonnegative
                x0 = abs(x0);
            end
            
            % Create observation
            y = AMatrix*x0;
            y = y + noiseVariance*randn(d,1);
            
            % Call L-1 solver
            tic
            switch l1Method
                case METHOD_OMP
                    [xp, iterationCount] = SolveOMP(AMatrix, y, ...
                        'maxIteration', maxIteration, ...
                        'isNonnegative', isNonnegative, ...
                        'lambda', lambda, ...
                        'stoppingcriterion', stoppingCriterion, ...
                        'groundtruth', x0, ...
                        'tolerance', tolerance);                
                case METHOD_BP 
                    [xp, iterationCount] = SolvePDIPA(AMatrix, y, ...
                        'maxIteration', maxIteration, ...
                        'isNonnegative', isNonnegative, ...
                        'lambda', lambda, ...
                        'stoppingcriterion', stoppingCriterion, ...
                        'groundtruth', x0, ...
                        'tolerance', tolerance);
                case METHOD_HOMOTOPY 
                    [xp, iterationCount] = SolveHomotopy(AMatrix, y, ...
                        'maxIteration', maxIteration,...
                        'isNonnegative', isNonnegative, ...
                        'stoppingCriterion', stoppingCriterion, ...
                        'groundtruth', x0, ...
                        'lambda', lambda, ...
                        'tolerance', tolerance);
                case METHOD_PFP 
                    [xp, iterationCount] = SolvePFP(AMatrix, y, ...
                        'maxIteration', maxIteration, ...
                        'isNonnegative', isNonnegative, ...
                        'stoppingCriterion', stoppingCriterion, ...
                        'groundtruth', x0, ...
                        'tolerance', tolerance);
                case METHOD_L1LS 
                    [xp, iterationCount] = SolveL1LS(AMatrix, y, ...
                        'maxIteration', maxIteration, ...
                        'lambda', lambda, ...
                        'stoppingCriterion', stoppingCriterion, ...
                        'groundtruth', x0, ...
                        'tolerance', tolerance);
                case METHOD_SpaRSA 
                     [xp, iterationCount] = SolveSpaRSA(AMatrix, y, ...
                        'maxIteration', maxIteration,...
                        'isNonnegative', isNonnegative, ...
                        'stoppingCriterion', stoppingCriterion,...
                        'groundtruth', x0, ...
                        'lambda', lambda, ...
                        'tolerance', tolerance);                   
                case METHOD_FISTA 
                    [xp, iterationCount] = SolveFISTA(AMatrix, y, ...
                        'maxIteration', maxIteration,...
                        'stoppingCriterion', stoppingCriterion,...
                        'groundtruth', x0, ...
                        'tolerance', tolerance,...
                        'lambda', lambda);     
                case METHOD_ALM 
                    [xp, iterationCount] = SolveDALM_fast(AMatrix, y, ...
                        'maxIteration', maxIteration,...
                        'stoppingCriterion', stoppingCriterion,...
                        'groundtruth', x0, ...
                        'tolerance', tolerance,...
                        'lambda', lambda);                                        
            end
            
            % Collect statistics
            currentRunTime = currentRunTime + toc;
            currentIteration = currentIteration + iterationCount;
            currentL2Error = currentL2Error + norm(x0-xp);
            
            if DEBUG>0
                
                disp(['L1 Method: ' num2str(l1Method) ', Average run time: ' num2str(currentRunTime/tryIndex) ', Average error: '...
                    num2str(currentL2Error/tryIndex)]);
                if DEBUG==2
                    figure;
                    set(gcf, 'Color', 'White');
                    
                    subplot(2,1,1);
                    bar(x0);
                    axis([0 2000 -0.3 0.3]);
                    
                    subplot(2,1,2);
                    bar(xp);
                    axis([0 2000 -0.3 0.3]);
                    
                    disp('The simulation is stopped. To continue, please assign DEBUG=1.')
                    return;
                end
            end
        end
        
        averageRunTime(dimensionIndex) = currentRunTime / tryCount;
        averageIteration(dimensionIndex) = currentIteration / tryCount;
        averageL2Error(dimensionIndex) = currentL2Error / tryCount;
        
        save l1benchmark.mat averageRunTime averageIteration averageL2Error
    end
end
