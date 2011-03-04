% Copyright ©2010. The Regents of the University of California (Regents). 
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

%% This routine is modified from Matlab Package "SparseLab"

function [sol, numIters] = SolvePFP(A, y, varargin)
% SolvePFP: Implements the Polytope Faces Pursuit algorithm
% Usage
%	[sols, numIters, activationHist] = SolvePFP(A, y, N, algType, maxIters,
%	verbose, OptTol)
% Input
%	A           An nxN matrix, with rank(A) = min(N,n).
%	y           vector of length n.
%   algType     'lars' for the Lars algorithm,
%               'pfp' for Polytope Faces Pursuit (default).
%               Add prefix 'nn' (i.e. 'nnlars' or 'nnpfp') to add a
%               non-negativity constraint (omitted by default)
%	maxIters    maximum number of Lars iterations to perform. If not
%               specified, runs to stopping condition (default)
%   solFreq     if =0 returns only the final solution, if >0, returns an
%               array of solutions, one every solFreq iterations (default 0).
%   verbose     1 to print out detailed progress at each iteration, 0 for
%               no output (default)
%	OptTol      Error tolerance, default 1e-5
% Outputs
%	sols           solution of the PFP algorithm
%	numIters       Total number of steps taken
%
% Description
%   SolvePFP implements the Polytope Faces Pursuit algorithm, to
%   solve the problem
%      min || x ||_1  s.t. Ax = y
%   via its dual
%      max y'*v s.t. A'*v <= 1
%   The implementation implicitly factors the active set matrix A(:,I)
%   using Choleskly updates.
% References
%   M.D. Plumbley, "Recovery of Sparse Representations by Polytope Faces
%   Pursuit", In Proceedings of the 6th International Conference on
%   Independent Component Analysis and Blind Source Separation (ICA 2006),
%   Charleston, SC, USA, 5-8 March 2006, LNCS 3889, pp 206-213.
%   Springer-Verlag, Berlin, 2006
% See Also
%   SolveOMP, SolveBP, SolveStOMP, SolveLasso
%

[n, N] = size(A);

tolerance = 1e-5;
verbose = 0;
solFreq = 0;
maxIters = 100;
algType = 'pfp';
nonNegative = false;
xG = [];
x0 = zeros(N,1);

STOPPING_GROUND_TRUTH = -1;
STOPPING_DUALITY_GAP = 1;
STOPPING_SPARSE_SUPPORT = 2;
STOPPING_OBJECTIVE_VALUE = 3;
STOPPING_SUBGRADIENT = 4;
STOPPING_DEFAULT = STOPPING_OBJECTIVE_VALUE;

stoppingCriterion = STOPPING_DEFAULT;

% Parse the optional inputs.
if (mod(length(varargin), 2) ~= 0 ),
    error(['Extra Parameters passed to the function ''' mfilename ''' must be passed in pairs.']);
end
parameterCount = length(varargin)/2;

for parameterIndex = 1:parameterCount,
    parameterName = varargin{parameterIndex*2 - 1};
    parameterValue = varargin{parameterIndex*2};
    switch lower(parameterName)
        case 'initialization'
            x0 = parameterValue;
            if ~all(size(x0)==[n,1])
                error('The dimension of the initial x0 does not match.');
            end            
        case 'stoppingcriterion'
            stoppingCriterion = parameterValue;
        case 'groundtruth'
            xG = parameterValue;
        case 'tolerance'
            tolerance = parameterValue;
        case 'algtype'
            algType = lower(parameterValue);
            if ~strcmp(algType,'pfp') && ~strcmp(algType,'lars')
                error(['The parameter value of ''' parameterName ''' is not recognized by the function ''' mfilename '''.']);
            end
        case 'maxiteration'
            maxIters = parameterValue;
        case 'isnonnegative'
            nonNegative = parameterValue;
        otherwise
            error(['The parameter ''' parameterName ''' is not recognized by the function ''' mfilename '''.']);
    end
end
clear varargin

if stoppingCriterion==STOPPING_GROUND_TRUTH && isempty(xG)
    error('The stopping criterion must provide the ground truth value of x.');
end

if ~nonNegative
    A = [A -A];
    negativeIndices = find(x0<0);
    x0 = [x0; zeros(N,1)];
    x0(N+negativeIndices) = -x0(negativeIndices);
    x0(negativeIndices) = 0;
    N = 2*N;
    
    if ~isempty(xG)
        negativeIndices = find(xG<0);
        xG = [xG; zeros(N,1)];
        xG(negativeIndices+N)=-xG(negativeIndices);
        xG(negativeIndices) = 0;
    end
end

if strcmp(algType,'lars')
    isLars = 1;
else
    isLars = 0;
end

% Global variables for linsolve function
global opts opts_tr errTol
opts.UT = true;
opts_tr.UT = true; opts_tr.TRANSA = true;
errTol = 1e-5;

k = 0;
v = zeros(n,1);
res = (y - A*x0);
R_I = [];
activeSet = find(abs(x0)>10*eps);
activationHist = activeSet;
collinearIndices = [];
done = 0;
prev_activeSet = [];
obj = 0.5*norm(y)^2;
while  ~done
    k = k+1;
    
    % Find next face
    inactiveSet = 1:N;
    inactiveSet(activeSet) = 0;
    inactiveSet(collinearIndices) = 0;
    inactiveSet = find(inactiveSet > 0);
    
    Ares = A(:,inactiveSet)'*res;
    Av = A(:,inactiveSet)'*v;
    
    % compute stopping criteria and test for termination
    keep_going = true;
    switch stoppingCriterion
        case STOPPING_GROUND_TRUTH
        	keep_going = norm(xG(inactiveSet))>tolerance;
        case STOPPING_SPARSE_SUPPORT
            % compute the stopping criterion based on the change
            % of the number of non-zero components of the estimate
            num_nz_x = length(activeSet);
            num_changes_active = length(setdiff(activeSet, prev_activeSet));
            if ~isempty(activeSet)
                criterionActiveSet = num_changes_active / num_nz_x;
                keep_going = (criterionActiveSet > tolerance);
            end
        case STOPPING_DUALITY_GAP
            dual = res/max(Ares);
            dualityGap = 0.5*norm(res)^2 + 0.5*(dual.'*dual) + y.'*dual;
            keep_going =  (dualityGap > tolerance);
        case STOPPING_OBJECTIVE_VALUE
            if ~isempty(activeSet)
                prev_obj = obj;
                obj = 0.5*norm(res)^2;
                keep_going = (abs((prev_obj-obj)/prev_obj) > tolerance);
            end
        case STOPPING_SUBGRADIENT
            error('Subgradient is not a valid stopping criterion for PFP.');
        otherwise,
            error('Undefined stopping criterion.');
    end % end of the stopping criteria switch
    
    if ~keep_going
        k = k-1;
        done = 1;
        break;
    else
        posInd = find(Ares > 0);
        [alpha_p,face_p] = max(Ares(posInd) ./ (1 - Av(posInd)));
        newFace = inactiveSet(posInd(face_p));
    end
    
    % Add new face to active set
    % Update the Cholesky factorization of A_I
    [R_I, flag] = updateChol(R_I, n, N, A, activeSet, newFace);
    % Check for collinearity
    if (flag)
        collinearIndices = [collinearIndices newFace];
        if verbose
            fprintf('Iteration %d: Variable %d is collinear\n', iter, newIndices(j));
        end
        valid = 1;
    else
        activeSet = [activeSet newFace];
        activationHist = [activationHist newFace];
        if verbose
            fprintf('Iteration %d: Adding variable %d\n', k, newFace);
        end
        valid = 0;
    end
    
    % Find first active vector to violate sign constraint
    while ~valid
        % Compute the solution estimate - solve the equation (A_I'*A_I)x_I = y
        corr_I = A(:,activeSet)'*y;
        z = linsolve(R_I,corr_I,opts_tr);
        x_I = linsolve(R_I,z,opts);
        
        ind = find(x_I < 0);
        if (length(ind) > 0) && (~isLars)
            col = ind(1);
            
            if verbose
                fprintf('Iteration %d: Dropping variable %d\n', k, activeSet(ind));
            end
            
            % If violating element is the one just added, ignore it in the
            % next iteration to avoid cycling
            if (activeSet(ind) == newFace)
                collinearIndices = [collinearIndices ind];
            else
                collinearIndices = [];
            end
            
            % Downdate the Cholesky factorization of A_I
            R_I = downdateChol(R_I,col);
            activationHist = [activationHist -activeSet(ind)];
            activeSet = [activeSet(1:col-1), activeSet(col+1:length(activeSet))];
        else
            valid = 1;
        end
    end
    
    z = linsolve(R_I,ones(length(activeSet),1),opts_tr);
    z = linsolve(R_I,z,opts);
    v = A(:,activeSet)*z;
    y_k = A(:,activeSet)*x_I;
    res = y - y_k;
    
    if k >= maxIters
        done = 1;
    end
end

x = zeros(N,1);
x(activeSet) = x_I;
if ~nonNegative
    sol = x(1:N/2) - x(N/2+1:N);
    activationHist = (mod(abs(activationHist)-1,N/2)+1) .* sign(activationHist);
else
    sol = x;
end
numIters = k;

clear opts opts_tr errTol


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [R, flag] = updateChol(R, n, N, A, activeSet, newIndex)
% updateChol: Updates the Cholesky factor R of the matrix
% A(:,activeSet)'*A(:,activeSet) by adding A(:,newIndex)
% If the candidate column is in the span of the existing
% active set, R is not updated, and flag is set to 1.

global opts_tr errTol
flag = 0;

newVec = A(:,newIndex);

if length(activeSet) == 0,
    R = sqrt(sum(newVec.^2));
else
    p = linsolve(R,A(:,activeSet)'*A(:,newIndex),opts_tr);
    q = sum(newVec.^2) - sum(p.^2);
    if (q <= errTol) % Collinear vector
        flag = 1;
    else
        R = [R p; zeros(1, size(R,2)) sqrt(q)];
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function R = downdateChol(R, j)
% downdateChol: `Downdates' the cholesky factor R by removing the
% column indexed by j.

% Remove the j-th column
R(:,j) = [];
[m,n] = size(R);

% R now has nonzeros below the diagonal in columns j through n.
% We use plane rotations to zero the 'violating nonzeros'.
for k = j:n
    p = k:k+1;
    [G,R(p,k)] = planerot(R(p,k));
    if k < n
        R(p,k+1:n) = G*R(p,k+1:n);
    end
end

% Remove last row of zeros from R
R = R(1:n,:);

