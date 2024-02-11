bool checkInteriorExterior(const cv::Mat &mask, const cv::Rect &croppingMask,
                                 int &top, int &bottom, int &left, int &right)
{
    // Return true if the rectangle is fine as it is
    bool result = true;

    cv::Mat sub = mask(croppingMask);
    int x = 0;
    int y = 0;

    // Count how many exterior pixels are, and choose that side for
    // reduction where mose exterior pixels occurred (that's the heuristic)

    int top_row = 0;
    int bottom_row = 0;
    int left_column = 0;
    int right_column = 0;

    for (y = 0, x = 0; x < sub.cols; ++x)
    {
        // If there is an exterior part in the interior we have
        // to move the top side of the rect a bit to the bottom
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++top_row;
        }
    }

    for (y = (sub.rows - 1), x = 0; x < sub.cols; ++x)
    {
        // If there is an exterior part in the interior we have
        // to move the bottom side of the rect a bit to the top
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++bottom_row;
        }
    }

    for (y = 0, x = 0; y < sub.rows; ++y)
    {
        // If there is an exterior part in the interior
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++left_column;
        }
    }

    for (x = (sub.cols - 1), y = 0; y < sub.rows; ++y)
    {
        // If there is an exterior part in the interior
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++right_column;
        }
    }

    // The idea is to set `top = 1` if it's better to reduce
    // the rect at the top than anywhere else.
    if (top_row > bottom_row)
    {
        if (top_row > left_column)
        {
            if (top_row > right_column)
            {
                top = 1;
            }
        }
    }
    else if (bottom_row > left_column)
    {
        if (bottom_row > right_column)
        {
            bottom = 1;
        }
    }

    if (left_column >= right_column)
    {
        if (left_column >= bottom_row)
        {
            if (left_column >= top_row)
            {
                left = 1;
            }
        }
    }
    else if (right_column >= top_row)
    {
        if (right_column >= bottom_row)
        {
            right = 1;
        }
    }

    return result;
}

bool compareX(cv::Point a, cv::Point b)
{
    return a.x < b.x;
}

bool compareY(cv::Point a, cv::Point b)
{
    return a.y < b.y;
}

void crop(cv::Mat &source)
{
    cv::Mat gray;
    source.convertTo(source, CV_8U);
    cvtColor(source, gray, cv::COLOR_RGB2GRAY);

    // Extract all the black background (and some interior parts maybe)

    cv::Mat mask = gray > 0;

    // now extract the outer contour
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
    cv::Mat contourImage = cv::Mat::zeros(source.size(), CV_8UC3);;

    // Find contour with max elements

    int maxSize = 0;
    int id = 0;

    for (int i = 0; i < contours.size(); ++i)
    {
        if (contours.at((unsigned long)i).size() > maxSize)
        {
            maxSize = (int)contours.at((unsigned long)i).size();
            id = i;
        }
    }

    // Draw filled contour to obtain a mask with interior parts

    cv::Mat contourMask = cv::Mat::zeros(source.size(), CV_8UC1);
    drawContours(contourMask, contours, id, cv::Scalar(255), -1, 8, hierarchy, 0, cv::Point());

    // Sort contour in x/y directions to easily find min/max and next

    std::vector<cv::Point> cSortedX = contours.at((unsigned long)id);
    std::sort(cSortedX.begin(), cSortedX.end(), compareX);
    std::vector<cv::Point> cSortedY = contours.at((unsigned long)id);
    std::sort(cSortedY.begin(), cSortedY.end(), compareY);

    int minXId = 0;
    int maxXId = (int)(cSortedX.size() - 1);
    int minYId = 0;
    int maxYId = (int)(cSortedY.size() - 1);

    cv::Rect croppingMask;

    while ((minXId < maxXId) && (minYId < maxYId))
    {
        cv::Point min(cSortedX[minXId].x, cSortedY[minYId].y);
        cv::Point max(cSortedX[maxXId].x, cSortedY[maxYId].y);
        croppingMask = cv::Rect(min.x, min.y, max.x - min.x, max.y - min.y);

        // Out-codes: if one of them is set, the rectangle size has to be reduced at that border

        int ocTop = 0;
        int ocBottom = 0;
        int ocLeft = 0;
        int ocRight = 0;

        bool finished = checkInteriorExterior(contourMask, croppingMask, ocTop, ocBottom, ocLeft, ocRight);

        if (finished == true)
        {
            break;
        }

        // Reduce rectangle at border if necessary

        if (ocLeft)
        { ++minXId; }
        if (ocRight)
        { --maxXId; }
        if (ocTop)
        { ++minYId; }
        if (ocBottom)
        { --maxYId; }
    }

    // Crop image with created mask

    source = source(croppingMask);
}