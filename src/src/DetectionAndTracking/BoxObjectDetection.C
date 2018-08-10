//============================================================================
// Name        : BoxObjectDetection.C
// Author      : Mayra Ochoa & Raymond Esteybar
// Version     :
// Copyright   :
// Description : Alternate version of ObjectDetection.H but BoxObjectDetection.H
//				 incorporates the Rectangles instead of Winner objects
//				 from .xml.
//============================================================================

#include "Component/OptionManager.H"
#include "Component/ParamClient.H"
#include "DetectionAndTracking/MbariFunctions.H"
#include "Image/ColorOps.H"
#include "Image/Image.H"
#include "Image/FilterOps.H"
#include "Image/PixelsTypes.H"
#include "Util/StringConversions.H"

#include "BoxObjectDetection.H"

class ModelParamBase;
class DetectionParameters;

BoxObjectDetection::BoxObjectDetection(OptionManager& mgr,
      const std::string& descrName,
      const std::string& tagName)
      : ModelComponent(mgr, descrName, tagName)
{

}
void BoxObjectDetection::start1(){

}
BoxObjectDetection::~BoxObjectDetection()
{}

void BoxObjectDetection::paramChanged(ModelParamBase* const param,
                                 const bool valueChanged,
                                 ParamClient::ChangeStatus* status){

}

std::list<BitObject> BoxObjectDetection::run(
    nub::soft_ref<MbariResultViewer> rv,
    const std::list<Creature> &creatureList,
    const Image< PixRGB<byte> > &segmentInImg)
{
    DetectionParameters p = DetectionParametersSingleton::instance()->itsParameters;
    std::list<BitObject> bosFiltered;
    std::list<BitObject> bosUnfiltered;
    std::list<Creature>::const_iterator iter = creatureList.begin();
    int minArea = p.itsMinEventArea;
    int maxArea = p.itsMaxEventArea;

    //go through each creature and extract objects
    while (iter != creatureList.end()) {

		// Creature's values
		Rectangle region = (*iter).getDimensions();
		std::string className = (*iter).getName();
		float classProbability = (*iter).getProbability();

		Point2D<int> unusedSeed;
		std::list<BitObject> sobjsKeep = extractBitObjects(segmentInImg, unusedSeed, region, region, minArea, maxArea);

		int sobjsSize = sobjsKeep.size();

		// add to the list
		LINFO("sobjsKeep.size() = %lu", sobjsKeep.size());
        bosUnfiltered.splice(bosUnfiltered.begin(), sobjsKeep);

		LINFO("Found %lu bitobject(s)", bosUnfiltered.size());

		// Set newly found BitObject values w/ Creature's values
		int i = 0;
		for(std::list<BitObject>::iterator it = bosUnfiltered.begin(); i < sobjsSize; ++it) {
			it->setClassProbability(className, classProbability);
			++i;
		}

		++iter;
    }

    bool found = false;
	int minSize = p.itsMinEventArea;
	if (p.itsRemoveOverlappingDetections) {
		LINFO("Removing overlapping detections");
		// loop until we find all non-overlapping objects starting with the smallest
		while (!bosUnfiltered.empty()) {

			std::list<BitObject>::iterator biter, siter, smallest;
			// find the smallest object
			smallest = bosUnfiltered.begin();
			for (siter = bosUnfiltered.begin(); siter != bosUnfiltered.end(); ++siter)
				if (siter->isValid() && siter->getArea() < minSize) {
					minSize = siter->getArea();
					smallest = siter;
				}

			// does the smallest object intersect with any of the already stored ones
			found = true;
			for (biter = bosFiltered.begin(); biter != bosFiltered.end(); ++biter) {
				if (smallest->isValid() && biter->isValid() && biter->doesIntersect(*smallest)) {
					// no need to store intersecting objects -> get rid of smallest
					// and look for the next smallest
					bosUnfiltered.erase(smallest);
					found = false;
					break;
				}
			}

			if (found && smallest->isValid()) {
				LDEBUG("### Name: %s | Probability: %f ###", smallest->getClassName(), smallest->getClassProbability());
				bosFiltered.push_back(*smallest);
			}
		}
	} else {
		std::list<BitObject>::iterator biter;
		for (biter = bosUnfiltered.begin(); biter != bosUnfiltered.end(); ++biter) {
			if (biter->isValid())
				bosFiltered.push_back(*biter);
		}
	}

    LINFO("Found total %lu objects", bosFiltered.size());
    return bosFiltered;
}

// ######################################################################
/* So things look consistent in everyone's emacs... */
/* Local Variables: */
/* indent-tabs-mode: nil */
/* End: */
