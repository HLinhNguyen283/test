call make singlehtml

set WKHTML_TO_PDF=%~dp0..\..\..\Tools\wkhtmltopdf\wkhtmltopdf.exe

for %%i in (.\_build\singlehtml\*.html) do (
	echo -------
	echo Found : %%~ni.html
	echo -------
	%WKHTML_TO_PDF% --user-style-sheet pdf.css %%~fi %%~ni.pdf
	goto end
)
:end
pause
