namespace InternFunctions {
	#include <stdio.h>
	#include <stdlib.h>
	#include <signal.h>

	#include <list>  // Change into a diff DS or make our own class
	#include <string>
	#include <fstream>
	#include <sstream>
	#include <iostream>
	#include <boost/algorithm/string.hpp>

	#include <xercesc/util/PlatformUtils.hpp>
	#include <xercesc/util/XMLString.hpp>
	#include <xercesc/parsers/AbstractDOMParser.hpp>
	#include <xercesc/dom/DOM.hpp>
	#include <xercesc/parsers/XercesDOMParser.hpp>

	#include "Media/FrameState.H"
	#include "Image/OpenCVUtil.H"
	#include "Channels/ChannelOpts.H"
	#include "Component/GlobalOpts.H"
	#include "Component/ModelManager.H"
	#include "Component/JobServerConfigurator.H"
	#include "Features/HistogramOfGradients.H"
	#include "Image/FilterOps.H"    // for lowPass3y()
	#include "Image/Kernels.H"      // for twofiftyfives()
	#include "Image/ColorOps.H"
	#include "Image/fancynorm.H"
	#include "Image/MorphOps.H"
	#include "Image/ShapeOps.H"   // for rescale()
	#include "Raster/GenericFrame.H"
	#include "Raster/PngWriter.H"
	#include "Media/FrameRange.H"
	#include "Media/FrameSeries.H"
	#include "Media/SimFrameSeries.H"
	#include "Media/MediaOpts.H"
	#include "Neuro/SpatialMetrics.H"
	#include "Neuro/StdBrain.H"
	#include "Neuro/NeuroOpts.H"
	#include "Neuro/Retina.H"
	#include "Neuro/VisualCortex.H"
	#include "Neuro/SimulationViewerStd.H"
	#include "SIFT/Histogram.H"
	#include "Simulation/SimEventQueueConfigurator.H"
	#include "Util/sformat.H"
	#include "Util/StringConversions.H"
	#include "Util/Pause.H"
	#include "Data/Logger.H"
	#include "Data/MbariMetaData.H"
	#include "Data/MbariOpts.H"

	#include "DetectionAndTracking/FOEestimator.H"
	#include "DetectionAndTracking/VisualEventSet.H"
	#include "DetectionAndTracking/DetectionParameters.H"
	#include "DetectionAndTracking/MbariFunctions.H"
	#include "DetectionAndTracking/Segmentation.H"
	#include "DetectionAndTracking/ColorSpaceTypes.H"
	#include "DetectionAndTracking/ObjectDetection.H"
	#include "DetectionAndTracking/Preprocess.H"
	#include "Image/MbariImage.H"
	#include "Image/MbariImageCache.H"
	#include "Image/BitObject.H"
	#include "Image/DrawOps.H"
	#include "Image/MathOps.H"
	#include "Image/IO.H"
	#include "Learn/BayesClassifier.H"
	#include "Media/MbariResultViewer.H"
	#include "Motion/MotionEnergy.H"
	#include "Motion/MotionOps.H"
	#include "Motion/OpticalFlow.H"
	#include "Util/StringConversions.H"
	#include "Utils/Version.H"
	#include "Component/ModelManager.H"
	#include "DetectionAndTracking/DetectionParameters.H"
	#include "Simulation/SimEventQueueConfigurator.H"
	#include "DetectionAndTracking/BoxObjectDetection.H"
	#include "Creature.H"

	using namespace std;
	using namespace xercesc;

	struct BoundingBox {
		int xmin;
		int ymin;
		int xmax;
		int ymax;
	};

	// global variables
	// Purpose: Store percent rescaled
	double rescalePercentWidth;
	double rescalePercentHeigth;

	// Main Functions
	// 	- Stores value in temporary 'Creature' Object
	void storeValue(BoundingBox& temp, const string& tagName, const string& tagValue) {    // Stores value in temporary 'Creature' Object
		// Convert Bounding Box Value from 'string to int'
		istringstream is(tagValue);
		int dim = 0;
		is >> dim;

		if(tagName == "xmin") {
			temp.xmin = dim;
		} else if(tagName == "ymin") {
			temp.ymin = dim;
		} else if(tagName == "xmax") {
			temp.xmax = dim;
		} else if(tagName == "ymax") {
			temp.ymax = dim;
		}
	}
	int getRescaleValues(const string& value){
		istringstream is(value);
		int val = 0;
		is >> val;

		return val;
	}
	// 	- Get .xml in directory
	string getmyXML(const string& descrip, const uint& fnum){
		vector<string>image_path;

		boost::split(image_path, descrip, boost::is_any_of("/"));

		vector<string>filename;
		boost::split(filename, image_path[image_path.size()-1], boost::is_any_of("#"));

		string fs = sformat("%s%06d.xml", filename[0].c_str() , fnum);

		string xml_path="";
		for(int i=1; i< image_path.size()-1; i++){
			xml_path.append(image_path[i]);
			xml_path.append("/");
		}
		xml_path.append(fs);
		return xml_path;
	}
	// 	- Reads .xml for values in <size> ... </size>
	void getImageSize(XercesDOMParser *itsParser,const int& resWidth,const int& resHeight){
		DOMNodeList *list = NULL;
		DOMDocument *domDocParser = itsParser->getDocument();

		XMLCh *source = XMLString::transcode("size"); 		// Tag wanted
		list = domDocParser->getElementsByTagName(source);		// Returns list of '<tag>' found
		int originalWidth = 0, originalHeight = 0;
		for(int i = 0; i < list->getLength(); ++i){
			DOMNode *node = list->item(i); 						// Gets the ith <object> in the list
			DOMNodeList *length = node->getChildNodes(); 		// Lines counted, including: "<object> ... </object>" = 13 lines total
			for(int k = 0; k < length->getLength(); ++k) {
				DOMNode *childNode = length->item(k);

				if(childNode->getNodeType() == DOMNode::ELEMENT_NODE) {						// Ensures we found a <tag>
					string tagNameObj = XMLString::transcode(childNode->getNodeName());		// <Gets Tag Name>
					string tagValueObj = XMLString::transcode(childNode->getTextContent());	// <tag> Gets Value </tag>
					istringstream is(tagValueObj);
					if(tagNameObj =="width"){
						is >> originalWidth;
						cout << "width: " << originalWidth <<  endl;
					} else if(tagNameObj =="height"){
						is >> originalHeight;
						cout << "height: " <<  originalHeight <<  endl;
					}
				}
			}
		}
		if(originalWidth!=resWidth && originalHeight!=resHeight){
			double result = (1.0 - (double(originalWidth-resWidth)/originalWidth));
			rescalePercentWidth = result;
			double result2 = (1.0 - (double(originalHeight-resHeight)/originalHeight));
			rescalePercentHeigth = result2;
			cout << "Rescale percentage: " << rescalePercentWidth << "% x " << rescalePercentHeigth << "%"<< endl;
		} else{
			rescalePercentWidth = 1.0;
			rescalePercentHeigth = 1.0;
		}
	}



	// 	- Reads .xml for values in <object> ... </object>
	void getObjectValues(XercesDOMParser *itsParser, list<Creature>& creatureList) {
		DOMNodeList *list = NULL;
		DOMDocument *domDocParser = itsParser->getDocument();

		// How many instances of the '<tag>' found
		XMLCh *source = XMLString::transcode("object"); 		// Tag wanted
		list = domDocParser->getElementsByTagName(source);		// Returns list of '<tag>' found

		// Parse through each object to grab values
		for(int i = 0; i < list->getLength(); ++i) {

			DOMNode *node = list->item(i); 						// Gets the ith <object> in the list
			DOMNodeList *length = node->getChildNodes(); 		// Lines counted, including: "<object> ... </object>" = 13 lines total

			BoundingBox tempDim;
			Creature tempCreature;
			Rectangle invalidRect;
			tempCreature.setDimensions(invalidRect);

			// Iterate through ea. <tag> in <object> ... </object> to retrieve values
			for(int k = 0; k < length->getLength(); ++k) {
				DOMNode *childNode = length->item(k);

				if(childNode->getNodeType() == DOMNode::ELEMENT_NODE) {						// Ensures we found a <tag>
					string tagNameObj = XMLString::transcode(childNode->getNodeName());		// <Gets Tag Name>
					string tagValueObj = XMLString::transcode(childNode->getTextContent());	// <tag> Gets Value </tag>

					// Grab Bounding Box Dimensions
					//	- Otherwise, get the <name>
					if(tagNameObj == "bndbox") {
						DOMNodeList *dimensions = childNode->getChildNodes();						// Gets all the <tags> in <bndbox>

						for(int j = 0; j < dimensions->getLength(); ++j) {							// Iterate each for dim. value
							DOMNode *dim = dimensions->item(j);

							if(dim->getNodeType() == DOMNode::ELEMENT_NODE) {
								string tagNameBB = XMLString::transcode(dim->getNodeName());		// <Gets Tag Name>
								string tagValueBB = XMLString::transcode(dim->getTextContent());	// <tag> Gets Value </tag>

								storeValue(tempDim, tagNameBB, tagValueBB);							// Store the dim values 1 by 1
							}
						}
						if(int(tempDim.xmin * rescalePercentWidth) != 0 && int(tempDim.ymin * rescalePercentHeigth) != 0)
							if(int(tempDim.xmin * rescalePercentWidth) > int(tempDim.ymin * rescalePercentHeigth))
								tempCreature.setDimensions(Rectangle::tlbrO(int(tempDim.ymin * rescalePercentHeigth), int(tempDim.xmin * rescalePercentWidth), int(tempDim.ymax * rescalePercentHeigth), int(tempDim.xmax * rescalePercentWidth)));	// Store creature found b/c dim are the last values to collect

						cout << "updated " << int(tempDim.xmin * rescalePercentWidth) << " " << int(tempDim.ymin * rescalePercentHeigth)<< endl;
						break;
					} else if(tagNameObj =="name"){
						cout << "Name: " <<  tagValueObj <<  endl;
						tempCreature.setName(tagValueObj);

					} else if(tagNameObj == "confidence") {

						// Convert Confidence value from 'string to int'
						istringstream is(tagValueObj);
						double con = 0;
						is >> con;

						tempCreature.setProbability(con);
					}

				}
			}

			if(tempCreature.getDimensions().isValid())
				creatureList.push_back(tempCreature);
		}

		cout << "#### Creature List Size: " << creatureList.size() << " #####\n";
	}
}

