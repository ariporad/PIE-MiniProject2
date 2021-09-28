function [fitresult, gof] = createFit(measuredFiltered, actualFiltered)
%CREATEFIT(MEASUREDFILTERED,ACTUALFILTERED)
%  Create a fit.
%
%  Data for 'distance_from_data' fit:
%      X Input : measuredFiltered
%      Y Output: actualFiltered
%  Output:
%      fitresult : a fit object representing the fit.
%      gof : structure with goodness-of fit info.
%
%  See also FIT, CFIT, SFIT.

%  Auto-generated by MATLAB on 17-Sep-2021 10:26:27


%% Fit: 'distance_from_data'.
[xData, yData] = prepareCurveData( measuredFiltered, actualFiltered );

% Set up fittype and options.
ft = fittype( 'exp1' );
opts = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts.Display = 'Off';
opts.StartPoint = [160.905831540222 -0.00439662441852814];

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft, opts );

% Plot fit with data.
figure( 'Name', 'distance_from_data' );
title("Analog output vs. Distance")
h = plot( fitresult, xData, yData );
legend( h, 'actualFiltered vs. measuredFiltered', 'distance_from_data', 'Location', 'NorthEast', 'Interpreter', 'none' );
% Label axes
xlabel( 'measuredFiltered', 'Interpreter', 'none' );
ylabel( 'actualFiltered', 'Interpreter', 'none' );
grid on


