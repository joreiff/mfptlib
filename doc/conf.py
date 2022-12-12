# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

# pylint: disable=invalid-name

import mfptlib


# -- General configuration ------------------------------------------------

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.extlinks',
    'sphinx.ext.intersphinx',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
]

templates_path = ['_templates']

source_suffix = '.rst'
master_doc = 'index'

project = 'mfptlib'
author = mfptlib.__author__
copyright = mfptlib.__copyright__ # pylint: disable=redefined-builtin
release = mfptlib.__version__
version = '.'.join(release.split('.')[:2])

language = 'en'
today_fmt = '%Y-%m-%d'

exclude_patterns = ['Thumbs.db', '.DS_Store']

modindex_common_prefix = ['mfptlib.']

autodoc_default_options = {
    'members': True,
    'undoc-members': True,
    'special-members': True,
    'exclude-members': '__annotations__, __dict__, __module__, __weakref__',
    'show-inheritance': True,
}
autodoc_member_order = 'groupwise'
autodoc_class_signature = 'separated'
autodoc_typehints_format = 'short'

pygments_style = 'default'

git_cfg = {
    'host': 'github.com',
    'user': 'joreiff',
    'repo': 'mfptlib',
    'branch': 'main',
}

git_base_url = 'https://{host}/{user}/{repo}/blob/{branch}'.format(**git_cfg)
extlinks = {
    'git-file': (git_base_url + '/%s', '%s'),
}

intersphinx_mapping = {
    'numpy': ('https://numpy.org/doc/stable/', None),
    'pybind11': ('https://pybind11.readthedocs.io/en/stable/', None),
    'python': ('https://docs.python.org/3', None),
}



# -- Options for HTML output ----------------------------------------------

html_theme = 'sphinx_rtd_theme'

html_theme_options = {}

html_context = {
    'display_github': True,
    'github_host': git_cfg['host'],
    'github_user': git_cfg['user'],
    'github_repo': git_cfg['repo'],
    'github_version': git_cfg['branch'],
    'conf_py_path': '/doc/',
}

# html_logo = 'img/mfptlib.png'

html_static_path = ['_static']
html_last_updated_fmt = '%Y-%m-%d %H:%M'
html_use_smartypants = True

htmlhelp_basename = 'mfptlibdoc'


# -- Options for LaTeX output ---------------------------------------------

latex_engine = 'lualatex'
latex_elements = {
    'papersize': 'a4paper',
    'pointsize': '11pt',
    'babel': '\\usepackage{babel}',
}

# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'mfptlib.tex', project + ' Documentation', author, 'manual'),
]

# latex_logo = 'img/mfptlib.png'

latex_show_pagerefs = True


# -- Options for manual page output ---------------------------------------

# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'mfptlib', project + ' Documentation', [author], 1)
]


# -- Setup ----------------------------------------------------------------

def setup(app):
    app.add_object_type('cmakevar', 'cmakevar', 'pair: %s; CMake variable')
