/*
 * BoxObjectDetection.C
 *
 *  Created on: Jul 2, 2018
 *      Author: mochoa
 */

#include "Component/OptionManager.H"
#include "Component/ParamClient.H"
#include "DetectionAndTracking/MbariFunctions.H"
#include "Image/ColorOps.H"
#include "Image/Image.H"
#include "Image/FilterOps.H"
#include "Image/PixelsTypes.H"
#include "Util/StringConversions.H"

class ModelParamBase;
class DetectionParameters;

#include "BoxObjectDetection.H"



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
// ======> TODO: Pass in list of custom class/struct that encapsulates rect, class name/probability here
std::list<BitObject> BoxObjectDetection::run(
    nub::soft_ref<MbariResultViewer> rv,
    const std::list<Rectangle> &rec,
    const Image< PixRGB<byte> > &segmentInImg)
{
    DetectionParameters p = DetectionParametersSingleton::instance()->itsParameters;
    std::list<BitObject> bosFiltered;
    std::list<BitObject> bosUnfiltered;
    std::list<Rectangle>::const_iterator iter = rec.begin();
    int minArea = p.itsMinEventArea;
    int maxArea = p.itsMaxEventArea;

    //go through each winner and extract salient objects
    while (iter != rec.end()) {

		//list of boxes
		//replace with our box
		Rectangle region = (*iter);

		//check if this list is empty
		Point2D<int> unusedSeed;
		std::list<BitObject> sobjsKeep = extractBitObjects(segmentInImg, unusedSeed, region, region, minArea, maxArea);
        // add to the list
        bosUnfiltered.splice(bosUnfiltered.begin(), sobjsKeep);

        //if list is empty and this is a high probability class, take the rectangle or box
        if(rec.empty()){
			Image<byte> foamask;
			BitObject bo;
			bo.reset(makeBinary(foamask,byte(1),byte(1),byte(1)));

			++iter;
        }

		LINFO("Found %lu bitobject(s)", bosUnfiltered.size());

		int minSize = 0;
		LINFO("Finding largest object");
		// loop until we find the largest bit object
		std::list<BitObject>::iterator biter, siter, largest;
		// find the largest object
		largest = bosUnfiltered.begin();
		for (siter = bosUnfiltered.begin(); siter != bosUnfiltered.end(); ++siter)
			if (siter->getArea() > minSize) {
				minSize = siter->getArea();
				largest = siter;
			}
		if (largest->isValid()) {
			//largest->setClassProbability(className, classProbability);
			bosFiltered.push_back(*largest);
		}

		++iter;
    }
    LINFO("Found total %lu objects", bosFiltered.size());
    return bosFiltered;
}

// ######################################################################
/* So things look consistent in everyone's emacs... */
/* Local Variables: */
/* indent-tabs-mode: nil */
/* End: */
