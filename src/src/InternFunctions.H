namespace InternFunctions {
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

	// 	- Reads .xml for values in <object> ... </object>
	void getObjectValues(XercesDOMParser *itsParser, list<Rectangle>& creatureDims, list<float>& confidencePer) {
		DOMNodeList *list = NULL;
		DOMDocument *domDocParser = itsParser->getDocument();

		// How many instances of the '<tag>' found
		XMLCh *source = XMLString::transcode("object"); 		// Tag wanted
		list = domDocParser->getElementsByTagName(source);		// Returns list of '<tag>' found

		// Parse through each object to grab values
		for(int i = 0; i < list->getLength(); ++i) {

			DOMNode *node = list->item(i); 						// Gets the ith <object> in the list
			DOMNodeList *length = node->getChildNodes(); 		// Lines counted, including: "<object> ... </object>" = 13 lines total

			BoundingBox temp;

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

								storeValue(temp, tagNameBB, tagValueBB);							// Store the dim values 1 by 1
							}
						}

						creatureDims.push_back(Rectangle::tlbrI(temp.ymin, temp.xmin, temp.ymax, temp.xmax));	// Store creature found b/c dim are the last values to collect

						break;
					} else if(tagNameObj == "confidence") {

						// Convert Confidence value from 'string to int'
						istringstream is(tagValueObj);
						double dim = 0;
						is >> dim;

						confidencePer.push_back(dim);
					}
				}
			}

			if(confidencePer.empty())
				confidencePer.push_back(1);
		}
	}
}
