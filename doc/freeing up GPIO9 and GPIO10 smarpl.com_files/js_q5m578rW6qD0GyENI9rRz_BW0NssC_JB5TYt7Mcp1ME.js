(function ($) {

Drupal.behaviors.textarea = {
  attach: function (context, settings) {
    $('.form-textarea-wrapper.resizable', context).once('textarea', function () {
      var staticOffset = null;
      var textarea = $(this).addClass('resizable-textarea').find('textarea');
      var grippie = $('<div class="grippie"></div>').mousedown(startDrag);

      grippie.insertAfter(textarea);

      function startDrag(e) {
        staticOffset = textarea.height() - e.pageY;
        textarea.css('opacity', 0.25);
        $(document).mousemove(performDrag).mouseup(endDrag);
        return false;
      }

      function performDrag(e) {
        textarea.height(Math.max(32, staticOffset + e.pageY) + 'px');
        return false;
      }

      function endDrag(e) {
        $(document).unbind('mousemove', performDrag).unbind('mouseup', endDrag);
        textarea.css('opacity', 1);
      }
    });
  }
};

})(jQuery);
;
(function ($) {
  Drupal.behaviors.ws_tb ={
    scriptadded: false,

    attach: function(context, settings) {
      if (this.scriptadded) {
        twttr.widgets.load();
      } else {
        $('a.service-links-twitter-widget', context).each(function(){
          $(this).attr('href', $(this).attr('href').replace(/((?:counturl\=|^))http[s]*\%3A\/\//g, "$1"));
        });
        $.getScript(window.location.protocol + '//platform.twitter.com/widgets.js', function () {
          this.scriptadded = true;
        });
      }
    }
  }
})(jQuery);
;
(function ($) {
  Drupal.behaviors.ws_pb = {
    attach: function(context, settings) {
      var $buttons = $('a.service-links-pinterest-button', context).once('service-links-pinterest-button');
      $buttons.each(function(){
        $(this).attr('count-layout', Drupal.settings.ws_pb.countlayout);
      });
      $.getScript( '//assets.pinterest.com/js/pinit.js' );
    }
  }
})(jQuery);
;
(function ($) {
  Drupal.behaviors.ws_lsb = {
    attach: function (context, settings) {
      $('a.service-links-linkedin-share-button', context).each(function(){
        var script_obj = document.createElement('script');
        script_obj.type = 'IN/Share';
        script_obj.setAttribute("data-url", $(this).attr('href'));
        if (Drupal.settings.ws_lsb.countmode != '') {
          script_obj.setAttribute("data-counter", Drupal.settings.ws_lsb.countmode);
        }
        $(this).replaceWith(script_obj);
      });

      try {
        IN.init({
          onLoad: "Drupal.behaviors.ws_lsb.parse"
        });
      }
      catch(e) {
        if (window.console && window.console.log) {
          console.log(e);
        }
      }
    },

    parse: function(context) {
      try {
        IN.parse(context);
      }
      catch(e) {
        if (window.console && window.console.log) {
          console.log(e);
        }
      }
    }
  }
})(jQuery);
;
(function ($) {
   Drupal.behaviors.ws_gpo = {
    scriptadded: false,

    attach: function (context, settings) {
      $('a.service-links-google-plus-one', context).once(function(){
        var g_text = document.createElement('g:plusone');

        g_text.setAttribute('href', $(this).attr('href'));
        g_text.setAttribute('width', Drupal.settings.ws_gpo.width);

        if (Drupal.settings.ws_gpo.size != '') {
          g_text.setAttribute('size', Drupal.settings.ws_gpo.size);
        }
        if (Drupal.settings.ws_gpo.annotation != '') {
          g_text.setAttribute('annotation', Drupal.settings.ws_gpo.annotation);
        }
        if (Drupal.settings.ws_gpo.callback) {
          g_text.setAttribute('callback', Drupal.settings.ws_gpo.callback);
        }

        $(this).replaceWith(g_text);
      });

      if (this.scriptadded) {
        gapi.plusone.go();
      } else {
        var params = { parsetags: "explicit" };

        if (Drupal.settings.ws_gpo.lang != '') {
          params.lang = Drupal.settings.ws_gpo.lang;
        }

        window.___gcfg = params

        $.ajax({
          url: "https://apis.google.com/js/plusone.js",
          dataType: "script",
          cache: true,
          success: function () {
            this.scriptadded = true;
            gapi.plusone.go();
          }
        });
      }
    }  
  }
})(jQuery);
;
(function ($) {
  Drupal.behaviors.ws_fs = {
    scriptadded: false,

    attach: function (context, settings) {
      if (!this.scriptadded) {
        var fdiv_root, js, div_root ;
        var fjs = document.getElementsByTagName('script')[0];

        if (!document.getElementById('fb-root')) {
          div_root = document.createElement('div');
          div_root.id = 'fb-root';
          document.body.appendChild(div_root);
        }

        if (!document.getElementById('facebook-jssdk')) {
          js = document.createElement('script'); 
          js.id = 'facebook-jssdk';
          js.src = "//connect.facebook.net/" + Drupal.settings.ws_fs.locale + "/all.js#xfbml=1&appId=" + Drupal.settings.ws_fs.app_id;
          fjs.parentNode.insertBefore(js, fjs);
          this.scriptadded = true;
        }
      }

      $('a.service-links-facebook-share', context).once(function(){
        var f_text = document.createElement('fb:share-button');
        var css = Drupal.settings.ws_fs.css.split(';');
        var key_value = new Array();

        f_text.setAttribute('type', Drupal.settings.ws_fs.type);
        f_text.setAttribute('href', $(this).attr('rel'));

        for (i = 0; i < css.length; i++){
          key_value = css[i].split(':');
          $(f_text).css(key_value[0], key_value[1]);
        }

        $(this).replaceWith(f_text);
      });
    }
  }
})(jQuery);
;
(function ($) {

/**
 * Automatically display the guidelines of the selected text format.
 */
Drupal.behaviors.filterGuidelines = {
  attach: function (context) {
    $('.filter-guidelines', context).once('filter-guidelines')
      .find(':header').hide()
      .closest('.filter-wrapper').find('select.filter-list')
      .bind('change', function () {
        $(this).closest('.filter-wrapper')
          .find('.filter-guidelines-item').hide()
          .siblings('.filter-guidelines-' + this.value).show();
      })
      .change();
  }
};

})(jQuery);
;
/**
 * @file
 */

(function ($) {

  'use strict';

  Drupal.extlink = Drupal.extlink || {};

  Drupal.extlink.attach = function (context, settings) {
    if (!settings.hasOwnProperty('extlink')) {
      return;
    }

    // Strip the host name down, removing ports, subdomains, or www.
    var pattern = /^(([^\/:]+?\.)*)([^\.:]{1,})((\.[a-z0-9]{1,253})*)(:[0-9]{1,5})?$/;
    var host = window.location.host.replace(pattern, '$2$3$6');
    var subdomain = window.location.host.replace(host, '');

    // Determine what subdomains are considered internal.
    var subdomains;
    if (settings.extlink.extSubdomains) {
      subdomains = '([^/]*\\.)?';
    }
    else if (subdomain === 'www.' || subdomain === '') {
      subdomains = '(www\\.)?';
    }
    else {
      subdomains = subdomain.replace('.', '\\.');
    }

    // Build regular expressions that define an internal link.
    var internal_link = new RegExp('^https?://([^@]*@)?' + subdomains + host, 'i');

    // Extra internal link matching.
    var extInclude = false;
    if (settings.extlink.extInclude) {
      extInclude = new RegExp(settings.extlink.extInclude.replace(/\\/, '\\'), 'i');
    }

    // Extra external link matching.
    var extExclude = false;
    if (settings.extlink.extExclude) {
      extExclude = new RegExp(settings.extlink.extExclude.replace(/\\/, '\\'), 'i');
    }

    // Extra external link CSS selector exclusion.
    var extCssExclude = false;
    if (settings.extlink.extCssExclude) {
      extCssExclude = settings.extlink.extCssExclude;
    }

    // Extra external link CSS selector explicit.
    var extCssExplicit = false;
    if (settings.extlink.extCssExplicit) {
      extCssExplicit = settings.extlink.extCssExplicit;
    }

    // Define the jQuery method (either 'append' or 'prepend') of placing the icon, defaults to 'append'.
    var extIconPlacement = settings.extlink.extIconPlacement || 'append';

    // Find all links which are NOT internal and begin with http as opposed
    // to ftp://, javascript:, etc. other kinds of links.
    // When operating on the 'this' variable, the host has been appended to
    // all links by the browser, even local ones.
    // In jQuery 1.1 and higher, we'd use a filter method here, but it is not
    // available in jQuery 1.0 (Drupal 5 default).
    var external_links = [];
    var mailto_links = [];
    $('a:not([data-extlink]), area:not([data-extlink])', context).each(function (el) {
      try {
        var url = '';
        if (typeof this.href == 'string') {
          url = this.href.toLowerCase();
        }
        // Handle SVG links (xlink:href).
        else if (typeof this.href == 'object') {
          url = this.href.baseVal;
        }
        if (url.indexOf('http') === 0
          && ((!url.match(internal_link) && !(extExclude && url.match(extExclude))) || (extInclude && url.match(extInclude)))
          && !(extCssExclude && $(this).is(extCssExclude))
          && !(extCssExclude && $(this).parents(extCssExclude).length > 0)
          && !(extCssExplicit && $(this).parents(extCssExplicit).length < 1)) {
          external_links.push(this);
        }
        // Do not include area tags with begin with mailto: (this prohibits
        // icons from being added to image-maps).
        else if (this.tagName !== 'AREA'
          && url.indexOf('mailto:') === 0
          && !(extCssExclude && $(this).parents(extCssExclude).length > 0)
          && !(extCssExplicit && $(this).parents(extCssExplicit).length < 1)) {
          mailto_links.push(this);
        }
      }
      // IE7 throws errors often when dealing with irregular links, such as:
      // <a href="node/10"></a> Empty tags.
      // <a href="http://user:pass@example.com">example</a> User:pass syntax.
      catch (error) {
        return false;
      }
    });

    if (settings.extlink.extClass) {
      Drupal.extlink.applyClassAndSpan(external_links, settings.extlink.extClass, extIconPlacement);
    }

    if (settings.extlink.mailtoClass) {
      Drupal.extlink.applyClassAndSpan(mailto_links, settings.extlink.mailtoClass, extIconPlacement);
    }

    if (settings.extlink.extTarget) {
      // Apply the target attribute to all links.
      $(external_links).attr('target', settings.extlink.extTarget);
      // Add rel attributes noopener and noreferrer.
      $(external_links).attr('rel', function (i, val) {
        // If no rel attribute is present, create one with the values noopener and noreferrer.
        if (val == null) {
          return 'noopener noreferrer';
        }
        // Check to see if rel contains noopener or noreferrer. Add what doesn't exist.
        if (val.indexOf('noopener') > -1 || val.indexOf('noreferrer') > -1) {
          if (val.indexOf('noopener') === -1) {
            return val + ' noopener';
          }
          if (val.indexOf('noreferrer') === -1) {
            return val + ' noreferrer';
          }
          // Both noopener and noreferrer exist. Nothing needs to be added.
          else {
            return val;
          }
        }
        // Else, append noopener and noreferrer to val.
        else {
          return val + ' noopener noreferrer';
        }
      });
    }

    Drupal.extlink = Drupal.extlink || {};

    // Set up default click function for the external links popup. This should be
    // overridden by modules wanting to alter the popup.
    Drupal.extlink.popupClickHandler = Drupal.extlink.popupClickHandler || function () {
      if (settings.extlink.extAlert) {
        return confirm(settings.extlink.extAlertText);
      }
    };

    $(external_links).click(function (e) {
      return Drupal.extlink.popupClickHandler(e, this);
    });
  };

  /**
   * Apply a class and a trailing <span> to all links not containing images.
   *
   * @param {object[]} links
   *   An array of DOM elements representing the links.
   * @param {string} class_name
   *   The class to apply to the links.
   * @param {string} icon_placement
   *   'append' or 'prepend' the icon to the link.
   */
  Drupal.extlink.applyClassAndSpan = function (links, class_name, icon_placement) {
    var $links_to_process;
    if (Drupal.settings.extlink.extImgClass) {
      $links_to_process = $(links);
    }
    else {
      var links_with_images = $(links).find('img').parents('a');
      $links_to_process = $(links).not(links_with_images);
    }
    // Add data-extlink attribute.
    $links_to_process.attr('data-extlink', '');
    var i;
    var length = $links_to_process.length;
    for (i = 0; i < length; i++) {
      var $link = $($links_to_process[i]);
      if ($link.css('display') === 'inline' || $link.css('display') === 'inline-block') {
        if (Drupal.settings.extlink.extUseFontAwesome) {
          if (class_name === Drupal.settings.extlink.mailtoClass) {
            $link[icon_placement]('<span class="fa-' + class_name + ' extlink"><span class="fa fa-envelope-o" title="' + Drupal.settings.extlink.mailtoLabel + '"></span><span class="element-invisible">' + Drupal.settings.extlink.mailtoLabel + '</span></span>');
          }
          else {
            $link[icon_placement]('<span class="fa-' + class_name + ' extlink"><span class="fa fa-external-link" title="' + Drupal.settings.extlink.extLabel + '"></span><span class="element-invisible">' + Drupal.settings.extlink.extLabel + '</span></span>');
          }
        }
        else {
          if (class_name === Drupal.settings.extlink.mailtoClass) {
            $link[icon_placement]('<span class="' + class_name + '"><span class="element-invisible">' + Drupal.settings.extlink.mailtoLabel + '</span></span>');
          }
          else {
            $link[icon_placement]('<span class="' + class_name + '"><span class="element-invisible">' + Drupal.settings.extlink.extLabel + '</span></span>');
          }
        }
      }
    }
  };

  Drupal.behaviors.extlink = Drupal.behaviors.extlink || {};
  Drupal.behaviors.extlink.attach = function (context, settings) {
    // Backwards compatibility, for the benefit of modules overriding extlink
    // functionality by defining an "extlinkAttach" global function.
    if (typeof extlinkAttach === 'function') {
      extlinkAttach(context);
    }
    else {
      Drupal.extlink.attach(context, settings);
    }
  };

})(jQuery);
;
