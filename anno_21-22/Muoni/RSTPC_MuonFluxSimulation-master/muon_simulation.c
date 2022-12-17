// =================================================================== //
//                                                                     //
// Author: R. Berner                                                   //
// Last edited: 10.08.2018                                             //
//                                                                     //
// This script simulates the cosmic muon flux through the              //
// Resistive Stell TPC, triggered by two (top and bottom)              //
// plastic scintillators with each having three SiPMs mounted          //
// on it. The scintillator tales are 70mm x 70mm in width and          //
// length (and 4mm in height). For each scintillator, a position       //
// dependent efficiency can be defined.                                //
// The active volume of the TPC is 53mm x 53mm x 150mm.                //
// The readout is done with two wire planes (collection & induction)   //
// with each consisting of 32 wires (separated by a wire pitch         //
// of 53mm/31).                                                        //
// We assume a cos^2(theta) PDF of the muon flux and use a             //
// cumulative distribution function (CDF) through integration          //
// over theta (zenith angle).                                          //
// The azimuth angle (phi) is uniformly distributed.                   //
//                                                                     //
// This script can be executed with ROOT version 6.06.08.              //
//                                                                     //
// =================================================================== //


// Some standard includes
// ==================================================
#include <iostream>
#include <math.h>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;


// Function declarations
// ==================================================
double cumulative_cos2(double input_value);

void muon_simulation(){

    // Define some ROOT stuff
    // ==================================================
    //gROOT.ProcessLine(".x /home/rberner/nd280work/.rootlogin.C");
    gStyle->SetPadLeftMargin(0.12); //0.18
    gStyle->SetPadBottomMargin(0.10); //0.16
    gStyle->SetPadTopMargin(0.07); //0.16 //1.05,"Y"
    gStyle->SetPadRightMargin(0.15);
    TGaxis::SetMaxDigits(5);
    //gStyle->SetTextSize(0.1);
    //gStyle->SetLabelSize(0.1);
    //gStyle->SetTitleSize(0.1);
    //gStyle->SetTitleFontSize(0.1);
    //gROOT->Reset();
    //TH1::SetDefaultSumw2();


    // Define simulation parameters
    // ==================================================
    unsigned n_tracks                   = 10000;       // number of muon tracks to simulate (~1 min for 60000 events)


    // Define dimensions and positions of the setup (in mm)
    // ==================================================
    int     dist_top_bottom_scint       =  235;       // [mm]
    double 	dist_top_scint_wires        =   48.5;     // [mm]
    int     timing_resol                = 4096;       // number of divisions
    int     length_tpc                  =  150;       // [mm]
    double  dist_bottom_scint_cathode   =   36.5;     // [mm] From middle (in z-direction) of the bottom scintillator to the cathode plane
    double  dist_bottom_scint_anode     =  186.5;     // [mm] From middle (in z-direction) of the bottom scintillator to the anode plane
    int     top_scint_length            =   70;       // Length: in x direction
    int     top_scint_width             =   70;       // Width: in y direction
    int     top_scint_z_pos             =  235;       // From middle of scintillator height
    int     bottom_scint_length         =   70;       // Length: in x direction
    int     bottom_scint_width          =   70;       // Width: in y direction
    int     bottom_scint_z_pos          =    0;       // From middle of scintillator height

    double  wire_plane                  =   53;       // Separation in mm between the first and the last wire in the plane
    int     n_wires                     =   32;
    double  wire_pitch                  =   wire_plane / (n_wires-1);
    double  active_vol_x_min            =   0. + (top_scint_length - n_wires * wire_pitch) / 2;
    double  active_vol_x_max            =   0. + (top_scint_length - n_wires * wire_pitch) / 2 + n_wires * wire_pitch;
    double  active_vol_y_min            =   0. + (top_scint_width - n_wires * wire_pitch) / 2;
    double  active_vol_y_max            =   0. + (top_scint_width - n_wires * wire_pitch) / 2 + n_wires * wire_pitch;
    double  active_vol_z_min            =   0. + dist_bottom_scint_cathode;
    double  active_vol_z_max            =   0. + dist_bottom_scint_anode;

    std::cout << " -------------------------------------------------------------------- " << std::endl;
    std::cout << " Active volume: x_min [mm]: " << active_vol_x_min << " \t x_max [mm]: " << active_vol_x_max << std::endl;
    std::cout << " Active volume: y_min [mm]: " << active_vol_y_min << " \t y_max [mm]: " << active_vol_y_max << std::endl;
    std::cout << " Active volume: z_min [mm]: " << active_vol_z_min << " \t z_max [mm]: " << active_vol_z_max << std::endl;
    std::cout << " -------------------------------------------------------------------- " << std::endl;


    // Define PDE (photon detection efficiencies) of the scintillators // ONLY ADJUST THE VALUES HERE
    // ==================================================
    int n_eff_bins_top_scint_x      = 7;
    int n_eff_bins_top_scint_y      = 7;
    int n_eff_bins_bottom_scint_x   = 7;
    int n_eff_bins_bottom_scint_y   = 7;
    double efficiencies_top_scint[7][7]     = { {1.00, 0.98, 0.96, 0.94, 0.92, 0.90, 0.88},
                                                {0.86, 0.84, 0.82, 0.80, 0.78, 0.76, 0.74},
                                                {0.72, 0.70, 0.68, 0.66, 0.64, 0.62, 0.60},
                                                {0.58, 0.56, 0.54, 0.52, 0.50, 0.48, 0.46},
                                                {0.44, 0.42, 0.40, 0.38, 0.36, 0.34, 0.32},
                                                {0.30, 0.28, 0.26, 0.24, 0.22, 0.20, 0.18},
                                                {0.16, 0.14, 0.12, 0.10, 0.08, 0.06, 0.04} };
    double efficiencies_bottom_scint[7][7]  = { {1.00, 0.98, 0.96, 0.94, 0.92, 0.90, 0.88},
                                                {0.86, 0.84, 0.82, 0.80, 0.78, 0.76, 0.74},
                                                {0.72, 0.70, 0.68, 0.66, 0.64, 0.62, 0.60},
                                                {0.58, 0.56, 0.54, 0.52, 0.50, 0.48, 0.46},
                                                {0.44, 0.42, 0.40, 0.38, 0.36, 0.34, 0.32},
                                                {0.30, 0.28, 0.26, 0.24, 0.22, 0.20, 0.18},
                                                {0.16, 0.14, 0.12, 0.10, 0.08, 0.06, 0.04} };


    // Set random number generator seed
    // ==================================================
    srand(time(NULL));


    // Get (normalized) cumulative distribution of cos^2(x) function in interval -pi/2 .. +pi/2
    // ==================================================
    // Note: Integral of cos^2(x)dx = 0.5*(x+sin(x)*cos(x)) + const.
    int n_steps = 1e7;
    TH1F * cumulative = new TH1F("cumulative","cumulative",n_steps,-90,90);
    for(int x=0; x<n_steps; x++) {
        cumulative->SetBinContent(x+1, 2/M_PI*cumulative_cos2(x*M_PI/n_steps - 0.5*M_PI)); // start with bin 1 because bin 0 is underflow bin
    }
    TCanvas *c000 = new TCanvas("c000","Canvas",1000,800);
    cumulative->SetStats(0);
    cumulative->SetTitle("");
    cumulative->GetXaxis()->SetTitle("#theta [degree]");
    cumulative->GetYaxis()->SetTitle("cumulated value");
    cumulative->GetXaxis()->SetTitleOffset(1.5);
    cumulative->GetYaxis()->SetTitleOffset(1.7);
    cumulative->Draw();
    c000->SaveAs("plots/cumulative_distribution_cos2(x)_large.root");
    c000->SaveAs("plots/cumulative_distribution_cos2(x)_large.png");


    // Define histogram
    // ==================================================
    TH2F * hist_xy_top_scint            = new TH2F("xy_top_scint","xy_top_scint",70,0,top_scint_length,70,0,top_scint_width);
    TH1F * hist_theta                   = new TH1F("cos2_angle","cos2_angle",90,-90,90);
    TH1F * hist_phi 	                = new TH1F("phi","phi",100,0,360);
    TH2F * hist_xy_in_bottom_scint      = new TH2F("hist_xy_in_bottom_scint",    "hist_xy_in_bottom_scint",    70,0,bottom_scint_length,70,0,bottom_scint_width);
    TH2F * hist_xy_not_in_bottom_scint  = new TH2F("hist_xy_not_in_bottom_scint","hist_xy_not_in_bottom_scint",100,-500,500,100,-500,500);

    int n_bins_x = n_wires;
    int n_bins_y = n_wires;
    int n_bins_z = length_tpc * 2; // At the moment: Bin size = 0.5 mm
    double voxel_size_x; // FILL THEM
    double voxel_size_y; // FILL THEM
    double voxel_size_z = (double) length_tpc / n_bins_z; // [mm]
    std::cout << " Voxel size z [mm]: " << voxel_size_z << std::endl;

    TH3I * hist_active_volume_voxels    = new TH3I("hist_active_volume_voxels","hist_active_volume_voxels",n_bins_x,active_vol_x_min,active_vol_x_max,n_bins_y,active_vol_y_min,active_vol_y_max,n_bins_z,active_vol_z_min,active_vol_z_max); // Histogram for the active volume of the TPC


    // Make simulation
    // ==================================================
    // Loop over all tracks
    for(int track=0; track<n_tracks; track++) {

        if(track%200==0) {
            std::cout << " Processing track " << track << " of " << n_tracks << std::endl;
        }

        // Get the (x,y) start position on the top scintillator
        std::vector<double> track_start(3,0);
        track_start[0] = ((double) rand() / (RAND_MAX)) * top_scint_length;
        track_start[1] = ((double) rand() / (RAND_MAX)) * top_scint_width;
        track_start[2] = top_scint_z_pos;

        // Simulate whether the top scintillator detects the light (according to the efficiencies_top_scint)
        double random_nr_top_scint    = ((double) rand() / (RAND_MAX));
        int x_bin_top = floor(track_start[0]/(top_scint_length / n_eff_bins_top_scint_x));
        int y_bin_top = floor(track_start[1]/(top_scint_width / n_eff_bins_top_scint_y));
        if (random_nr_top_scint > efficiencies_top_scint[x_bin_top][y_bin_top]) continue;

        // Get the random angle (following a cos^2 distribution)
        // NOTE: THIS COULD BE MADE FASTER IF CREATING A VECTOR WITH ALL BIN CONTENTS:
        // CREATE A RANDOM NUMBER IN INTERVAL [0,NBINS] AND DO: RANDOM-ANGLE = VECTOR[NBINS*RANDOMNUMBER]
        double rand_0 = ((double) rand() / (RAND_MAX));
        double theta  = -99.9;
        for(int bin=0; bin<n_steps ; bin++) {
            if( (cumulative->GetBinContent(bin+1)<rand_0) && (cumulative->GetBinContent(bin+2)>rand_0)) { theta = bin/1e7*180-90; }
        }

        // Get the direction of the track (2*pi symmetric)
        double phi = ((double) rand() / (RAND_MAX)) * 360.;
        //std::cout << " \t x [mm]: " << track_start[0] << " \t y [mm]: " << track_start[1] << " \t z [mm]: " << track_start[3] << " \t Angle theta [°]: " << theta << " \t phi [°]: " << phi << std::endl;


        // Fill histograms to check for correctness of the random variables
        hist_xy_top_scint ->Fill(track_start[0],track_start[1]);
        hist_theta        ->Fill(theta);
        hist_phi          ->Fill(phi);


        // Propagate a straight track from the start point on top of the scintillator
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        double track_length_between_scintillators = fabs(-(bottom_scint_z_pos - track_start[2]) / cos(theta));
        std::vector<double> track_end(3,0);
        track_end[0] = track_start[0] - track_length_between_scintillators*sin(theta)*cos(phi);
        track_end[1] = track_start[1] - track_length_between_scintillators*sin(theta)*sin(phi);
        track_end[2] = track_start[2] - fabs(track_length_between_scintillators*cos(theta));

        // Only take events which hit the bottom scintillator
        if( (track_end[0]<bottom_scint_length && track_end[0]>0) && (track_end[1]<bottom_scint_width && track_end[1]>0) ) {
            // Simulate whether the top scintillator detects the light (according to the efficiencies_top_scint)
            double random_nr_bottom_scint    = ((double) rand() / (RAND_MAX));
            int x_bin_bottom = floor(track_start[0]/(top_scint_length / n_eff_bins_top_scint_x));
            int y_bin_bottom = floor(track_start[1]/(top_scint_width / n_eff_bins_top_scint_y));
            if (random_nr_bottom_scint > efficiencies_bottom_scint[x_bin_bottom][y_bin_bottom]) continue;
            else hist_xy_in_bottom_scint->Fill(track_end[0],track_end[1]);
            //std::cout << " Track passed through both scintillators: " << std::endl;
            //std::cout << " Top scintillator:    x_start: " << track_start[0] << "   \t y_start: " << track_start[1] << " \t z_start: " << track_start[2] << " \t track length: " << track_length_between_scintillators << std::endl;
            //std::cout << " Bottom scintillator: x_end:   " << track_end[0]   << "   \t y_end: " << track_end[1]   << "   \t z_end: " << track_end[2]   << "   \t track length: " << track_length_between_scintillators << std::endl;
        }
        else {
            hist_xy_not_in_bottom_scint->Fill(track_end[0],track_end[1]);
            //std::cout << " Track not in bottom scintillator: x_end: " << track_end[0] << "   \t y_end: " << track_end[1] << "   \t z_end: " << track_end[2] << "   \t track length: " << track_length_between_scintillators << std::endl;
            continue;
        }


        // Propagate track to the z-coordinate-boundary of each voxel and set bin content of the neighboring voxels to 1
        std::vector<double> track_start_in_active_volume = {-999, -999, -999};
        double track_x_pos; // MAKE VECTORS!!
        double track_y_pos;
        double track_z_pos;
        for(int voxelplane=0; voxelplane<n_bins_z; voxelplane++) {
            track_start_in_active_volume[0] = track_start[0] - (top_scint_z_pos-active_vol_z_max)*sin(theta)*cos(phi);
            track_start_in_active_volume[1] = track_start[1] - (top_scint_z_pos-active_vol_z_max)*sin(theta)*sin(phi);
            track_start_in_active_volume[2] = track_start[2] - (top_scint_z_pos-active_vol_z_max);
            //if(voxelplane==0) { std::cout << " Track start in active volume: " << track_start_in_active_volume[0] << " \t" << track_start_in_active_volume[1] << " \t" << track_start_in_active_volume[2] << std::endl; }

            // Propagate track through the active volume. Always calculate the x-y position at the z-coordinate boundary of two voxels
            track_x_pos = track_start[0] - (top_scint_z_pos-active_vol_z_max + voxelplane*voxel_size_z)*sin(theta)*cos(phi);
            track_y_pos = track_start[1] - (top_scint_z_pos-active_vol_z_max + voxelplane*voxel_size_z)*sin(theta)*sin(phi);
            track_z_pos = track_start[2] - (top_scint_z_pos-active_vol_z_max + voxelplane*voxel_size_z);
            //std::cout << "    Track propagation through active volume:\t x: " << track_x_pos << " \t y: " << track_y_pos << " \t z: " << track_z_pos << std::endl;
            hist_active_volume_voxels->Fill(track_x_pos, track_y_pos, track_z_pos);
        }

    } // end loop over all tracks


    // Draw histograms
    TCanvas *c001 = new TCanvas("c001","Canvas",1000,800);
    TH2D * hist_projection_xz = (TH2D*)hist_active_volume_voxels->Project3D("zx");
    hist_projection_xz->SetStats(0);
    hist_projection_xz->SetTitle("Projection of the active volume to the xz plane");
    hist_projection_xz->GetXaxis()->SetTitle("x coord. [mm]");
    hist_projection_xz->GetYaxis()->SetTitle("z coord. [mm]");
    hist_projection_xz->Draw("colz");
    c001->SaveAs("plots/xz_projection_of_active_volume.root");
    c001->SaveAs("plots/xz_projection_of_active_volume.png");

    TCanvas *c002 = new TCanvas("c002","Canvas",1000,800);
    TH2D * hist_projection_yz = (TH2D*)hist_active_volume_voxels->Project3D("zy");
    hist_projection_yz->Draw("colz");
    hist_projection_yz->SetStats(0);
    hist_projection_yz->SetTitle("Projection of the active volume to the yz plane");
    hist_projection_yz->GetXaxis()->SetTitle("y coord. [mm]");
    hist_projection_yz->GetYaxis()->SetTitle("z coord. [mm]");
    hist_projection_yz->Draw("colz");
    c002->SaveAs("plots/yz_projection_of_active_volume.root");
    c002->SaveAs("plots/yz_projection_of_active_volume.png");

    TCanvas *c003 = new TCanvas("c003","Canvas",1000,800);
    TH2D * hist_projection_xy = (TH2D*)hist_active_volume_voxels->Project3D("yx");
    hist_projection_xy->Draw("colz");
    hist_projection_xy->SetStats(0);
    hist_projection_xy->SetTitle("Projection of the active volume to the xy plane");
    hist_projection_xy->GetXaxis()->SetTitle("x coord. [mm]");
    hist_projection_xy->GetYaxis()->SetTitle("y coord. [mm]");
    hist_projection_xy->Draw("colz");
    c003->SaveAs("plots/xy_projection_of_active_volume.root");
    c003->SaveAs("plots/xy_projection_of_active_volume.png");

    TCanvas *c004 = new TCanvas("c004","Canvas",1000,800);
    hist_xy_top_scint->SetStats(0);
    hist_xy_top_scint->SetTitle("Tracks start position on top scintillator");
    hist_xy_top_scint->GetXaxis()->SetTitle("top scintillator x coord. [mm]");
    hist_xy_top_scint->GetYaxis()->SetTitle("top scintillator y coord. [mm]");
    hist_xy_top_scint->GetXaxis()->SetTitleOffset(1.5);
    hist_xy_top_scint->GetYaxis()->SetTitleOffset(1.7);
	hist_xy_top_scint->Draw("COLZ");
    c004->SaveAs("plots/xy_top_scint.root");
    c004->SaveAs("plots/xy_top_scint.png");

	TCanvas *c005 = new TCanvas("c005","Canvas",1000,800);
    hist_theta->SetStats(0);
    hist_theta->SetTitle("Zenith angle of incident track on top scintillator");
    hist_theta->GetXaxis()->SetTitle("#theta [degree]");
    hist_theta->GetYaxis()->SetTitle("# entries");
    hist_theta->GetXaxis()->SetTitleOffset(1.5);
    hist_theta->GetYaxis()->SetTitleOffset(1.7);
	hist_theta->Draw();
    c005->SaveAs("plots/hist_theta.root");
    c005->SaveAs("plots/hist_theta.png");

    TCanvas *c006 = new TCanvas("c006","Canvas",1000,800);
    hist_phi->SetStats(0);
    hist_phi->SetTitle("Azimuth of incident track on top scintillator");
    hist_phi->GetXaxis()->SetTitle("phi [degree]");
    hist_phi->GetYaxis()->SetTitle("# entries");
    hist_phi->GetXaxis()->SetTitleOffset(1.5);
    hist_phi->GetYaxis()->SetTitleOffset(1.7);
    hist_phi->Draw();
    c006->SaveAs("plots/hist_phi.root");
    c006->SaveAs("plots/hist_phi.png");

    TCanvas *c007 = new TCanvas("c007","Canvas",1000,800);
    hist_xy_in_bottom_scint->SetStats(0);
    hist_xy_in_bottom_scint->SetTitle("Tracks passing through bottom scintillator");
    hist_xy_in_bottom_scint->GetXaxis()->SetTitle("bottom scintillator x coord. [mm]");
    hist_xy_in_bottom_scint->GetYaxis()->SetTitle("bottom scintillator y coord. [mm]");
    hist_xy_in_bottom_scint->GetXaxis()->SetTitleOffset(1.5);
    hist_xy_in_bottom_scint->GetYaxis()->SetTitleOffset(1.7);
	hist_xy_in_bottom_scint->Draw("COLZ");
    c007->SaveAs("plots/xy_in_bottom_scint.root");
    c007->SaveAs("plots/xy_in_bottom_scint.png");

    TCanvas *c008 = new TCanvas("c008","Canvas",1000,800);
    hist_xy_not_in_bottom_scint->SetStats(0);
    hist_xy_not_in_bottom_scint->SetTitle("Tracks missed bottom scintillator");
    hist_xy_not_in_bottom_scint->GetXaxis()->SetTitle("bottom scintillator x coord. [mm]");
    hist_xy_not_in_bottom_scint->GetYaxis()->SetTitle("bottom scintillator y coord. [mm]");
    hist_xy_not_in_bottom_scint->GetXaxis()->SetTitleOffset(1.5);
    hist_xy_not_in_bottom_scint->GetYaxis()->SetTitleOffset(1.7);
	hist_xy_not_in_bottom_scint->Draw("COLZ");
    c008->SaveAs("plots/xy_not_in_bottom_scint.root");
    c008->SaveAs("plots/xy_not_in_bottom_scint.png");

}

double cumulative_cos2(double input_value) {
	return 0.5*(input_value + sin(input_value)*cos(input_value)) - 0.5*(-0.5*M_PI+sin(-0.5*M_PI)*cos(-0.5*M_PI));
}
