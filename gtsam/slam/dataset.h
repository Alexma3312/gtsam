/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file dataset.h
 * @date Jan 22, 2010
 * @author nikai
 * @brief utility functions for loading datasets
 */

#pragma once


#include <string>
#include <gtsam/slam/pose2SLAM.h>
#include <gtsam/inference/graph.h>

namespace gtsam
{
/**
 * Construct dataset filename from short name
 * Currently has "Killian" "intel.gfs", "10K", etc...
 * @param filename
 * @param optional dataset, if empty will try to getenv $DATASET
 * @param optional path, if empty will try to getenv $HOME
 */
std::pair<std::string, boost::optional<gtsam::SharedDiagonal> >
	dataset(const std::string& dataset = "", const std::string& path = "");

/**
 * Load TORO 2D Graph
 * @param filename
 * @param maxID, if non-zero cut out vertices >= maxID
 * @param smart: try to reduce complexity of covariance to cheapest model
 */
std::pair<boost::shared_ptr<gtsam::Pose2Graph>, boost::shared_ptr<DynamicValues> > load2D(
		std::pair<std::string, boost::optional<SharedDiagonal> > dataset,
		int maxID = 0, bool addNoise=false, bool smart=true);
std::pair<boost::shared_ptr<gtsam::Pose2Graph>, boost::shared_ptr<DynamicValues> > load2D(
		const std::string& filename,
		boost::optional<gtsam::SharedDiagonal> model = boost::optional<gtsam::SharedDiagonal>(),
		int maxID = 0, bool addNoise=false, bool smart=true);

/** save 2d graph */
void save2D(const gtsam::Pose2Graph& graph, const DynamicValues& config, const gtsam::SharedDiagonal model,
		const std::string& filename);

/**
 * Load TORO 3D Graph
 */
bool load3D(const std::string& filename);

} // namespace gtsam
