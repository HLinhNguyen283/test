call make singlehtml

for %%i in (.\_build\singlehtml\*.html) do (
	start %%i
)
:end
pause
