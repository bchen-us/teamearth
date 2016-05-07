#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkDiscreteGaussianImageFilter.h"
// Software Guide : EndCodeSnippet
int main(int argc, char * argv[])
{
	if (argc < 5)
	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << "  inputImageFile  outputImageFile  variance  maxKernelWidth " << std::endl;
		return EXIT_FAILURE;
	}
	
	typedef    float    InputPixelType;
	typedef    float    OutputPixelType;
	typedef itk::Image< InputPixelType, 3 >   InputImageType;
	typedef itk::Image< OutputPixelType, 3 >   OutputImageType;
	// Software Guide : EndCodeSnippet
	typedef itk::ImageFileReader< InputImageType >  ReaderType;
	//  Software Guide : BeginLatex
	//
	//  The discrete Gaussian filter type is instantiated using the
	//  input and output image types.  A corresponding filter object is created.
	//
	//  \index{itk::DiscreteGaussianImageFilter!instantiation}
	//  \index{itk::DiscreteGaussianImageFilter!New()}
	//  \index{itk::DiscreteGaussianImageFilter!Pointer}
	//
	//  Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	typedef itk::DiscreteGaussianImageFilter<
		InputImageType, OutputImageType >  FilterType;
	FilterType::Pointer filter = FilterType::New();
	// Software Guide : EndCodeSnippet
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);
	//  Software Guide : BeginLatex
	//
	//  The input image can be obtained from the output of another
	//  filter. Here, an image reader is used as its input.
	//
	//  Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	filter->SetInput(reader->GetOutput());
	// Software Guide : EndCodeSnippet
	const double gaussianVariance = atof(argv[3]);
	const unsigned int maxKernelWidth = atoi(argv[4]);
	//  Software Guide : BeginLatex
	//
	//  The filter requires the user to provide a value for the variance
	//  associated with the Gaussian kernel. The method \code{SetVariance()} is
	//  used for this purpose. The discrete Gaussian is constructed as a
	//  convolution kernel.  The maximum kernel size can be set by the user. Note
	//  that the combination of variance and kernel-size values may result in a
	//  truncated Gaussian kernel.
	//
	//  \index{itk::DiscreteGaussianImageFilter!SetVariance()}
	//  \index{itk::DiscreteGaussianImageFilter!SetMaximumKernelWidth()}
	//
	//  Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	filter->SetVariance(gaussianVariance);
	filter->SetMaximumKernelWidth(maxKernelWidth);
	// Software Guide : EndCodeSnippet
	//  Software Guide : BeginLatex
	//
	//  Finally, the filter is executed by invoking the \code{Update()} method.
	//
	//  \index{itk::DiscreteGaussianImageFilter!Update()}
	//
	//  Software Guide : EndLatex
	// Software Guide : BeginCodeSnippet
	filter->Update();
	// Software Guide : EndCodeSnippet
	//  Software Guide : BeginLatex
	//
	//  If the output of this filter has been connected to other filters down
	//  the pipeline, updating any of the downstream filters will
	//  trigger the execution of this one. For example, a writer could
	//  be used after the filter.
	//
	//  Software Guide : EndLatex
	typedef unsigned char                          WritePixelType;
	typedef itk::Image< WritePixelType, 3 >        WriteImageType;
	typedef itk::RescaleIntensityImageFilter<
		OutputImageType, WriteImageType > RescaleFilterType;
	RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(255);
	typedef itk::ImageFileWriter< WriteImageType >  WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(argv[2]);
	// Software Guide : BeginCodeSnippet
	rescaler->SetInput(filter->GetOutput());
	writer->SetInput(rescaler->GetOutput());
	writer->Update();
	// Software Guide : EndCodeSnippet
	//  Software Guide : BeginLatex
	//
	// \begin{figure}
	// \center
	// \includegraphics[width=0.44\textwidth]{BrainProtonDensitySlice}
	// \includegraphics[width=0.44\textwidth]{DiscreteGaussianImageFilterOutput}
	// \itkcaption[DiscreteGaussianImageFilter output]{Effect of the
	// DiscreteGaussianImageFilter on a slice from a MRI proton density image of
	// the brain.}
	// \label{fig:DiscreteGaussianImageFilterInputOutput}
	// \end{figure}
	//
	//  Figure~\ref{fig:DiscreteGaussianImageFilterInputOutput} illustrates the
	//  effect of this filter on a MRI proton density image of the brain.
	//
	//  Note that large Gaussian variances will produce large convolution kernels
	//  and correspondingly longer computation times.  Unless a high degree of
	//  accuracy is required, it may be more desirable to use the approximating
	//  \doxygen{RecursiveGaussianImageFilter} with large variances.
	//
	//  Software Guide : EndLatex
	return EXIT_SUCCESS;
}